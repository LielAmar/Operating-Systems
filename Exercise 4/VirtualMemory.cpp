#include "PhysicalMemory.h"

// ===== API Functions ======
word_t mmu(uint64_t virtualAddress, int depth);

// ===== Helper Functions =====
int abs(int value) { return (value < 0 ? -value : value); }
int min(int var1, int var2) { return (var1 < var2 ? var1 : var2); }
int max(int var1, int var2) { return (var1 < var2 ? var2 : var1); }

/**
 * Checks if the given frame is in the ancestors (unable to use frames) list
 *
 * @param frame       Frame to check
 * @param ancestors   List of ancestors
 * @return            Whether the frame is an ancestor
 */
bool is_ancestor(word_t frame, const word_t ancestors[TABLES_DEPTH]) {
    for(int i = 0; i < TABLES_DEPTH; i++) {
        if(frame == ancestors[i]) {
            return true;
        }
    }

    return false;
}

/**
 * Resets the given frame
 *
 * @param frame   Frame to reset
 */
void reset_frame(word_t frame) {
    for(int j = 0; j < PAGE_SIZE; j++) {
        PMwrite(frame * PAGE_SIZE + j, 0);
    }
}

/**
 * Looks for every ancestor that points to {target_frame} and remove the link
 *
 * @param frame          starting frame to go down recursively from
 * @param depth          current recursion depth
 * @param target_frame   targeted frame to unlink from
 */
void remove_ancestor_link(word_t frame, int depth, word_t target_frame) {
    if(depth >= TABLES_DEPTH) {
        return;
    }

    word_t value;

    // Loop over all frames stored in the current frame and remove the link to target_frame when encountered
    for(int offset = 0; offset < PAGE_SIZE; offset++) {
        PMread(frame * PAGE_SIZE + offset, &value);

        if(value == target_frame) {
            PMwrite(frame * PAGE_SIZE + offset, 0);
        } else {
            remove_ancestor_link(value, depth + 1, target_frame);
        }
    }
}

/**
 * Returns whether the targeted frame is a leaf or not
 *
 * @param frame          starting frame to go down recursively from
 * @param depth          current recursion depth
 * @param target_frame   targeted frame to unlink from
 * @return               is the given frame a leaf
 */
bool is_leaf(word_t frame, int depth, word_t target_frame) {
    if(depth >= TABLES_DEPTH) {
        return false;
    }

    word_t value;

    for(int offset = 0; offset < PAGE_SIZE; offset++) {
        PMread(frame * PAGE_SIZE + offset, &value);

        if(value == 0) {
            continue;
        } else if(value == target_frame) {
            return (depth == TABLES_DEPTH - 1);
        } else if(is_leaf(value, depth + 1, target_frame)) {
            return true;
        }
    }

    return false;
}

/**
 * Returns the frame resulted when traversing through page
 *
 * Assumes page doesn't return to address 0
 *
 * @param page   Page to use to traverse the tables
 * @return       Frame address
 */
word_t get_frame_by_page(word_t page) {
    word_t addr = 0;

    for(int i = 0; i < TABLES_DEPTH; i++) {
        uint64_t page_index = page >> (OFFSET_WIDTH * (TABLES_DEPTH - i - 1));
        uint64_t innerOffset = (page_index) & (PAGE_SIZE - 1);

        PMread(addr * PAGE_SIZE + innerOffset, &addr);
    }

    return addr;
}

/**
 * Tries to find the maximum frame index used recursively.
 *
 * @param frame   starting frame to go down recursively from
 * @param depth   current recursion depth
 * @return        Maximum frame used within the page table
 */
word_t find_maximal_seen_frame(word_t frame, unsigned int depth) {
    if(depth >= TABLES_DEPTH) {
        return 0;
    }

    word_t max_value = 0, value;

    // Loop over all frames stored in the current frame and take the max one out of them, and their inner frames
    for(int offset = 0; offset < PAGE_SIZE; offset++) {
        // Check values within the current frame
        PMread(frame * PAGE_SIZE + offset, &value);
        max_value = max(value, max_value);

        // Check values within the frame that is the current value
        value = find_maximal_seen_frame(value, depth + 1);
        max_value = max(value, max_value);
    }

    return max_value;
}

/**
 * Tries to find the used page with the maximum cyclical distance from the page
 * we're trying to insert (page_swapped_in)
 *
 * @param frame             starting frame to go down recursively from
 * @param depth             current recursion depth
 * @param ancestors         list of ancestors to ignore and not evict
 * @param page_swapped_in   the page we're trying to insert
 * @param p                 the current page we're checking
 * @param best_p            the best page to evict we've found so far
 * @return                  the best page to evict
 */
word_t find_maximal_cyclical_distance(word_t frame, unsigned int depth, word_t ancestors[TABLES_DEPTH],
                                      word_t page_swapped_in, word_t p, word_t* best_p) {
    if(depth >= TABLES_DEPTH) {
        if(is_ancestor(get_frame_by_page(p), ancestors)) {
            return 0;
        }

        *best_p = p;
        return min(NUM_PAGES - abs(page_swapped_in - p), abs(page_swapped_in - p));
    }

    word_t max_value = 0;
    word_t value;

    // Loop over all frames stored in the current frame and take the max distanced one
    // out of all of them (except ancestors)
    for(int offset = 0; offset < PAGE_SIZE; offset++) {
        PMread(frame * PAGE_SIZE + offset, &value);

        if(value == 0) {
            continue;
        }

        // Check values within the frame that is the current value
        // 0000 0000 0000 0001
        word_t new_p = (p << OFFSET_WIDTH) | offset;
        word_t attempted_best_p;

        value = find_maximal_cyclical_distance(value, depth + 1, ancestors, page_swapped_in, new_p, &attempted_best_p);
        if(value > max_value) {
            max_value = value;
            *best_p = attempted_best_p;
        }
    }

    return max_value;
}


// ===== Frame Priority Functions =====
word_t find_empty_frame(word_t ancestors[TABLES_DEPTH]);
word_t find_unused_frame();
word_t evict_max_cyclical_distance_frame(word_t page_swapped_in, word_t ancestors[TABLES_DEPTH]);


/**
 * Finds a new frame that is free to use (by looking for an empty frame/evicting a frame)
 *
 * @param ancestors   List of ancestors to skip
 * @return            New frame that's free to use
 */
word_t find_frame_to_use(word_t page_swapped_in, word_t ancestors[TABLES_DEPTH]) {
    word_t frame;

    // 1st priority: Find an empty frame
    frame = find_empty_frame(ancestors);

    if(frame != -1) {
        return frame;
    }

    // 2nd priority: Find an unused frame
    frame = find_unused_frame();

    if(frame != -1) {
        return frame;
    }

    // 3rd priority: Evict max cyclical distance frame
    word_t page = evict_max_cyclical_distance_frame(page_swapped_in, ancestors);
    frame = get_frame_by_page(page);

    PMevict(frame, page);

    // We'd like to remove the connection between the parent (DDDD) and the parent
    // execute mmu without going to the actual frame but rather 1 step earlier
    word_t addr = mmu(page, TABLES_DEPTH - 1);
    uint64_t pageOffset = page & (PAGE_SIZE - 1);
    PMwrite(addr * PAGE_SIZE + pageOffset, 0);

    return frame;
}

/**
 * Tries to find an empty frame for usage (1st priority)
 *
 * @param ancestors  List of ancestors to skip
 * @return           Frame to use, or -1 if no empty frames
 */
word_t find_empty_frame(word_t ancestors[TABLES_DEPTH]) {
    word_t empty_frame = -1;

    for(word_t frame = 1; frame < NUM_FRAMES; frame++) {
        if(is_ancestor(frame, ancestors)) {
            continue;
        }

        word_t value;
        int zeros = 0;

        for(int offset = 0; offset < PAGE_SIZE; offset++) {
            PMread(frame * PAGE_SIZE + offset, &value);

            zeros += (value == 0 ? 1 : 0);
        }

        if(zeros == PAGE_SIZE && !is_leaf(0, 0, frame)) {
            empty_frame = frame;
            break;
        }
    }

    if(empty_frame == -1) {
        return -1;
    }

    // Remove ancestors of empty_frame
    remove_ancestor_link(0, 0, empty_frame);

    return empty_frame;
}

/**
 * Tries to find an unused (unreferenced) frame for usage (2nd priority)
 * That is, if the maximal frame referenced is not the possible maximum, there are unused pages, so return one.
 *
 * @return   An unused frame we can use
 */
word_t find_unused_frame() {
    word_t next_available_frame = find_maximal_seen_frame(0, 0) + 1;

    if(next_available_frame < NUM_FRAMES) {
        return next_available_frame;
    }

    return -1;
}

/**
 * Evicts the page with the max cyclical distance from the page we're trying to insert
 *
 * @param page_swapped_in   page we're trying to insert
 * @param ancestors         list of ancestors to not evict
 * @return                  the page we evcited
 */
word_t evict_max_cyclical_distance_frame(word_t page_swapped_in, word_t ancestors[TABLES_DEPTH]) {
    word_t best_p;
    find_maximal_cyclical_distance(0, 0, ancestors, page_swapped_in, 0, &best_p);

    return best_p;
}

/**
 * Finds the frame address by traversing the page table through the address virtualAddress
 * - If there's a page fault, this function solves it by calling #find_frame_to_use
 *
 * @param virtualAddress   virtual address to use
 * @param depth            max depth allowed (iterations over the page table)
 * @return                 the address of the frame we want to access
 */
word_t mmu(uint64_t virtualAddress, int depth) {
    // If the virtualAddress is beyond our limit
    if((virtualAddress >> VIRTUAL_ADDRESS_WIDTH) > 0) {
        return -1;
    }

    uint64_t current_page = virtualAddress >> OFFSET_WIDTH;
    word_t ancestors[TABLES_DEPTH] = {0};
    word_t addr = 0;


    // Traverse the tree (for TABLE_DEPTH steps), always reading exactly OFFSET_WIDTH bits representing
    // the next address
    for(int i = 0; i < depth; i++) {
        // Offset within current leaf is OFFSET_WIDTH bits from left to right in the i'th position
        // - (1000 0101 1010 1011 1111) -> [1000, 0101, ..., 1111]
        uint64_t page_index = virtualAddress >> (OFFSET_WIDTH * (depth - i));
        uint64_t innerOffset = (page_index) & (PAGE_SIZE - 1);

        word_t prev_addr = addr;
        PMread(addr * PAGE_SIZE + innerOffset, &addr);

        if(addr == 0) {
            word_t frame = find_frame_to_use(current_page, ancestors);

            if(frame == -1) {
                return -1;
            }

            PMwrite(prev_addr * PAGE_SIZE + innerOffset, frame);

            if(i < TABLES_DEPTH - 1) {
                reset_frame(frame);
                addr = frame;
            } else if(i == TABLES_DEPTH - 1) {
                PMrestore(frame, page_index);
                addr = frame;
            }
        }

        ancestors[i] = addr;
    }

    return addr;
}


// ===== API Functions =====
void VMinitialize() {
    // Resets the whole first (0) page with values 0
    reset_frame(0);
}

int VMread(uint64_t virtualAddress, word_t* value) {
    // If we received an invalid value
    if(value == nullptr) {
        return 0;
    }

    // If the table depth is 0, meaning we want to access the physical memory directly.
    if(TABLES_DEPTH == 0) {
        PMread(virtualAddress, value);
        return 1;
    }

    word_t addr = mmu(virtualAddress, TABLES_DEPTH);

    if(addr == -1) {
        return 0;
    }

    uint64_t pageOffset = virtualAddress & (PAGE_SIZE - 1);

    PMread(addr * PAGE_SIZE + pageOffset, value);
    return 1;
}

int VMwrite(uint64_t virtualAddress, word_t value) {
    // If the table depth is 0, meaning we want to access the physical memory directly.
    if(TABLES_DEPTH == 0) {
        PMwrite(virtualAddress, value);
        return 1;
    }

    word_t addr = mmu(virtualAddress, TABLES_DEPTH);

    if(addr == -1) {
        return 0;
    }

    uint64_t pageOffset = virtualAddress & (PAGE_SIZE - 1);

    PMwrite(addr * PAGE_SIZE + pageOffset, value);
    return 1;
}