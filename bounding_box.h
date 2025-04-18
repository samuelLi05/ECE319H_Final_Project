#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <stdint.h>
class bounding_box{
    private:
        uint32_t x, y, w, h;
    public:
        bool collision(bounding_box&); // reference so no need for copy constructor
        uint32_t get_x();
        uint32_t get_y();
        uint32_t get_w();
        uint32_t get_h();

        bounding_box();
        bounding_box(int, int, int, int);
        bounding_box(bounding_box&);
};


#endif // BOUNDING_BOX_H