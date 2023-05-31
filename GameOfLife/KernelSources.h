#pragma once

#include <string>

namespace game_of_life {

std::string vector_add_kernel = R"(

kernel void memset(   global uint *dst )
{          
    dst[get_global_id(0)] = get_global_id(0);
}

)";

std::string gof_kernel = R"(

uint getPixelValFromImg(__read_write image3d_t img_data, int x, int y)
{
    int4 cell_coord = (int4)(x, y, 0, 0);
    return ((uint)(read_imageui(img_data, cell_coord).x));
}

void checkDeadCells(__read_write image3d_t img_data, int x, int y)
{
    uint neighbour_cells = 0; 
    if (getPixelValFromImg(img_data, x - 1, y - 1) == 1) // BF
    {
        neighbour_cells++;
    }
    if (getPixelValFromImg(img_data, x - 1, y) == 1) // F
    {
        neighbour_cells++;
    }
    if (getPixelValFromImg(img_data, x - 1, y + 1) == 1) // JF
    {
        neighbour_cells++;
    }
    if (getPixelValFromImg(img_data, x, y - 1) == 1) // B
    {
        neighbour_cells++;
    }
    if (getPixelValFromImg(img_data, x, y + 1) == 1) // J
    {
        neighbour_cells++;
    }   
    if (getPixelValFromImg(img_data, x + 1, y - 1) == 1) // BA
    {
        neighbour_cells++;
    }
    if (getPixelValFromImg(img_data, x + 1, y) == 1) // A
    {
        neighbour_cells++;
    }
    if (getPixelValFromImg(img_data, x + 1, y + 1) == 1) // JA
    {
        neighbour_cells++;
    }
    if (neighbour_cells == 3)
    {
        int4 new_coord = (int4)(x, y, 1, 0);
        write_imageui(img_data, new_coord, (uint4)(1, 0, 0, 1));
    }
}

__kernel void gof_solve(__read_write image3d_t img_data, sampler_t img_sampler)
{
    
    uint xdim = get_global_id(0);
    uint ydim = get_global_id(1);
    uint zdim = get_global_id(2);

    // Read from cell (alive[1] or not[0] from the old space)
    int4 cell_coord = (int4)(xdim, ydim, 0, 0);
    uint cell_val = read_imageui(img_data, cell_coord).x;
    

    // Start neighbour test
    if (cell_val == 1)
    {
        uint neighbour_cells = 0; 
        if (getPixelValFromImg(img_data, xdim - 1, ydim - 1) == 1) // BF
        {
            neighbour_cells++;
        }
        else 
        {
            checkDeadCells(img_data, xdim - 1, ydim - 1);
        }
        if (getPixelValFromImg(img_data, xdim - 1, ydim) == 1) // F
        {
            neighbour_cells++;
        }
        else 
        {
            checkDeadCells(img_data, xdim - 1, ydim);
        }
        if (getPixelValFromImg(img_data, xdim - 1, ydim + 1) == 1) // JF
        {
            neighbour_cells++;
        }
        else 
        {
            checkDeadCells(img_data, xdim - 1, ydim + 1);
        }
        if (getPixelValFromImg(img_data, xdim, ydim - 1) == 1) // B
        {
            neighbour_cells++;
        }
        else 
        {
            checkDeadCells(img_data, xdim, ydim - 1);
        }
        if (getPixelValFromImg(img_data, xdim, ydim + 1) == 1) // J
        {
            neighbour_cells++;
        } 
        else 
        {
            checkDeadCells(img_data, xdim, ydim + 1);
        }
        if (getPixelValFromImg(img_data, xdim + 1, ydim - 1) == 1) // BA
        {
            neighbour_cells++;
        }
        else 
        {
            checkDeadCells(img_data, xdim + 1, ydim - 1);
        }
        if (getPixelValFromImg(img_data, xdim + 1, ydim) == 1) // A
        {
            neighbour_cells++;
        }
        else 
        {
            checkDeadCells(img_data, xdim + 1, ydim);
        }
        if (getPixelValFromImg(img_data, xdim + 1, ydim + 1) == 1) // JA
        {
            neighbour_cells++;
        }
        else 
        {
            checkDeadCells(img_data, xdim + 1, ydim + 1);
        }
        if (neighbour_cells == 2 || neighbour_cells == 3)
        {
            int4 new_coord = (int4)(xdim, ydim, 1, 0);
            write_imageui(img_data, new_coord, (uint4)(1, 0, 0, 1));
        }  

   }     
}

__kernel void gof_copy(__read_write image3d_t img_data, sampler_t img_sampler)
{
    
    uint xdim = get_global_id(0);
    uint ydim = get_global_id(1);
    //uint zdim = get_global_id(2);
    
    int4 old_coord = (int4)(xdim, ydim, 0, 0);
    int4 new_coord = (int4)(xdim, ydim, 1, 0); 
    //uint4 nullVec = (uint4)(0, 0, 0, 1);   
    uint4 newval = read_imageui(img_data, new_coord);
    write_imageui(img_data, old_coord, newval);
    //write_imageui(img_data, new_coord, nullVec);
    

}
__kernel void gof_clear(__read_write image3d_t img_data, sampler_t img_sampler)
{
    
    uint xdim = get_global_id(0);
    uint ydim = get_global_id(1);
    //uint zdim = get_global_id(2);
    
    //int4 old_coord = (int4)(xdim, ydim, 0, 0);
    int4 new_coord = (int4)(xdim, ydim, 1, 0); 
    uint4 nullVec = (uint4)(0, 0, 0, 1);   
    //uint4 newval = read_imageui(img_data, new_coord);
    //write_imageui(img_data, old_coord, newval);
    write_imageui(img_data, new_coord, nullVec);
    

}

)";

}