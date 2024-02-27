/******************************************************************************
* Project: Knight Tour                                                        *
*                                                                             *
* Wariten by : Eva De-Botton                                                  *
*                                                                             *
* Reviewd by : Liel Luko                                                      *
*                                                                             *
* Date : 3/11/2021                                                            *
*                                                                             *
******************************************************************************/

#include <stdlib.h>  /* malloc free           */
#include <assert.h>  /* assert                */
#include <math.h>    /* log                   */
#include <stdio.h>   /* printf                */
#include <time.h>    /* clock, CLOCKS_PER_SEC */ 

#include "knight_tour.h"
#include "bit_array.h"
#include "utils.h"

#define NUM_NEXT_MOVE        8
#define ROAD_IS_BLOCKED     255 
#define BOARD_SIZE          (DIM * DIM)

/********************** static for Knights Tour Warnsdorff ********************/

static stat_ty KnightsTourWarnsdorffRec(pos_ty position,
                                        bit_arr_ty open_positions,
                                        pos_ty route[BOARD_SIZE],int index,
                                        time_t start_t, time_t time_cap);

static unsigned char NextPositionWarnsdorff(size_t possible_moves,
                                            size_t open_positions);

/********************** static for Knights Tour Recursive *********************/

static stat_ty KnightsTourRecursive(pos_ty start_pos,
                                    bit_arr_ty open_positions,
                                    pos_ty route[BOARD_SIZE], int index,
                                    time_t start_t, time_t time_cap);

/*************************** static for Knights Tour **************************/

static size_t GetPossiblePositions(size_t open_positions, pos_ty position);
static size_t GetBiggesPosition(size_t possible_moves);
static size_t TurnOffPosition(size_t possible_moves, pos_ty position);
static size_t CountPositions(size_t possible_moves);

/*************************** static for Mark Visit ****************************/

static bit_arr_ty MarkAsVisit(bit_arr_ty open_positions, size_t position,
                              pos_ty route[BOARD_SIZE],  size_t index);

static bit_arr_ty MarkAsUnVisit(bit_arr_ty open_positions, size_t position,
                                pos_ty route[BOARD_SIZE],  size_t index);


/********************** static for Init Next Positio Lut **********************/

static void InitNextPositionLUT(void);
static void MarkToVisit(pos_ty offset, int j, int i);

/************************* static for Print Solution **************************/

static void BoardPrinter(pos_ty route[BOARD_SIZE]);
static void PrintSolution(stat_ty status, pos_ty route[BOARD_SIZE], 
                                          pos_ty start_pos);

/***************************** static variables *******************************/

static size_t possible_position_lut[BOARD_SIZE];
static int init_flag = OFF;

/*******************************************************************************
************************************ Init Pos *********************************/

pos_ty InitPos(int x, int y)
{
    return (y * DIM + x);
}

/*******************************************************************************
*********************************** Parse Pos *********************************/

void ParsePos(pos_ty pos, int *x, int *y)
{
    *x = pos % DIM; 
    *y = pos / DIM;

    return;
}
/*******************************************************************************
**************************** Knights Tour Warnsdorff **************************/

stat_ty KnightsTourWarnsdorff(pos_ty start_pos, pos_ty route[DIM * DIM],
                                                             time_t time_cap)
{
    bit_arr_ty open_positions = ~(0);
    pos_ty position = start_pos;

    int index = 0;

    stat_ty status = NOT_FOUND;

    time_t start_t = time(NULL);

    assert(NULL != route);
    assert(BOARD_SIZE > start_pos);

    InitNextPositionLUT();
    
    status = KnightsTourWarnsdorffRec(position, open_positions, route,
                                      index, start_t, time_cap);
    
    PrintSolution(status, route, start_pos); 
    
    return status;
}

/********************** static for Knights Tour Warnsdorff ********************/

static stat_ty KnightsTourWarnsdorffRec(pos_ty position,
                                        bit_arr_ty open_positions,
                                        pos_ty route[BOARD_SIZE],int index,
                                        time_t start_t, time_t time_cap)
{
    stat_ty status = NOT_FOUND;

    pos_ty next_position = 0;

    size_t possible_moves = 0;

    time_t current_t = time(NULL);

    if(time_cap < current_t - start_t)
    {
        return TIME_OUT;
    }

    if (index == BOARD_SIZE - 1)
    {
        MarkAsVisit(open_positions, position, route, index);
        return FOUND;
    }

    if(ROAD_IS_BLOCKED == position)
    {
        return NOT_FOUND;
    }

    open_positions = MarkAsVisit(open_positions, position, route, index);
    possible_moves = GetPossiblePositions(open_positions, position);

    while(0 != possible_moves)
    {
        next_position = NextPositionWarnsdorff(possible_moves,open_positions);

        status = KnightsTourWarnsdorffRec(next_position, open_positions,
                                          route, index + 1, start_t, time_cap);
        
        if(NOT_FOUND == status)
        {
            possible_moves = TurnOffPosition(possible_moves, next_position);
        }
        else if(TIME_OUT == status)
        {
            return TIME_OUT;
        }
        else
        {
            return FOUND;
        }
    }

    open_positions = MarkAsUnVisit(open_positions, position, route, index);
 
    return NOT_FOUND;
}

/******************************************************************************/

static unsigned char NextPositionWarnsdorff(size_t possible_moves,
                                            size_t open_positions)
{
    pos_ty warnsdorff_next_position = 0;
    pos_ty next_position = 0;

    size_t possible_moves_next = 0;
    size_t num_of_moves_next = 0; 
    size_t num_of_moves = CountPositions(possible_moves); 

    size_t min_moves = NUM_NEXT_MOVE + 1;

    if(0 == possible_moves)
    {
        return ROAD_IS_BLOCKED;
    }

    while(0 != possible_moves)
    {
        next_position = GetBiggesPosition(possible_moves); 

        possible_moves_next = GetPossiblePositions(open_positions, next_position);
        num_of_moves_next   = CountPositions(possible_moves_next);

        if (num_of_moves_next < min_moves && num_of_moves_next != 0)
        {
            warnsdorff_next_position = next_position;
            min_moves = num_of_moves_next;
        }
        else if (num_of_moves_next == 0 && num_of_moves == 1)
        {
            warnsdorff_next_position = next_position;
        }

        possible_moves = TurnOffPosition(possible_moves, next_position);  
    }

    return warnsdorff_next_position;
}




/*******************************************************************************
**************************** Knights Tour Recursive ***************************/

stat_ty KnightsTour(pos_ty start_pos, pos_ty route[DIM * DIM],
                                                             time_t time_cap)
{
    bit_arr_ty open_positions = ~(0);

    int index = 0;

    stat_ty status = NOT_FOUND;

    time_t start_t = time(NULL);

    assert(NULL != route);
    assert(BOARD_SIZE > start_pos);

    InitNextPositionLUT();

    status = KnightsTourRecursive(start_pos, open_positions, route, 
                                                 index, start_t, time_cap);
    
    PrintSolution(status, route, start_pos); 
    
    return status;
}


/********************** static for Knights Tour Recursive *********************/

static stat_ty KnightsTourRecursive(pos_ty position,
                                    bit_arr_ty open_positions,
                                    pos_ty route[BOARD_SIZE], int index,
                                    time_t start_t, time_t time_cap)
{
    stat_ty status = NOT_FOUND;

    pos_ty next_position = 0;

    size_t possible_moves = 0;

    time_t current_t = time(NULL);

    if(time_cap < current_t - start_t)
    {
        return TIME_OUT;
    }

    if (index == BOARD_SIZE - 1)
    {
        MarkAsVisit(open_positions, position, route, index);
        return FOUND;
    }

    if(ROAD_IS_BLOCKED == position)
    {
        return NOT_FOUND;
    }

    open_positions = MarkAsVisit(open_positions, position, route, index);
    possible_moves = GetPossiblePositions(open_positions, position);

    while(0 != possible_moves)
    {
        next_position = GetBiggesPosition(possible_moves);
        
        status = KnightsTourWarnsdorffRec(next_position, open_positions,
                                          route, index + 1, start_t, time_cap);
        
        if(NOT_FOUND == status)
        {
            possible_moves = TurnOffPosition(possible_moves, next_position);
        }
        else if(TIME_OUT == status)
        {
            return TIME_OUT;
        }
        else
        {
            return FOUND;
        }
    }

    open_positions = MarkAsUnVisit(open_positions, position, route, index);
 
    return NOT_FOUND;
}

/*************************** static for Knights Tour **************************/

/*  Get Possible Positions explanation:

            open_positions indicate which position the knight can move to:
                                
                                > set bit - has not yet passed 
                                > bit off - has already passed
                
                                at the beginnig all the bits are on.

                                011111111111111111101111111111011111

            possible_position_lut[position] indicate which position the knight 
                                can move from the current position:

                                > set bit - the position is legal for knight 
                                            & located inside the board
                                > bit off - the position is illegal

                                000000000100001000010001000010000000

           to determine where the knight can move, we need to choose a legal 
           position, that the knight has not yet passed

                                011111111111111111101110111111011111
                            &
                                000000000100001000010001000010000000

                            =
                                000000000100001000000000000010000000

            the solution give as the possible_moves.
*/

static size_t GetPossiblePositions(size_t open_positions, pos_ty position)
{
    assert(BOARD_SIZE > position);

    return (possible_position_lut[position] & open_positions);
}

/*
            in order to extrect the largest position from the bit array:

                                possible_moves
                            = 

                                000000000100001000000000000010000000

                                ____________________________________

                                log(2)(possible_moves) 

                            = 
                                000000000100000000000000000000000000
*/

static size_t GetBiggesPosition(size_t possible_moves)
{
    return ((size_t)(log((double)(possible_moves)) / log(2)));
}

static size_t TurnOffPosition(size_t possible_moves, pos_ty position)
{
    assert(BOARD_SIZE > position);
    
    return (BitsArrSetOff(possible_moves, position));
}

static size_t CountPositions(size_t possible_moves)
{
    return (BitsArrCountOn(possible_moves));
}

/*************************** static for Mark Visit ****************************/

static bit_arr_ty MarkAsVisit(bit_arr_ty open_positions,  size_t position,
                              pos_ty route[BOARD_SIZE], size_t index)
{
    route[index] = position;

    return BitsArrSetOff(open_positions, position);
}

/******************************************************************************/

static bit_arr_ty MarkAsUnVisit(bit_arr_ty open_positions,  size_t position,
                                pos_ty route[BOARD_SIZE], size_t index)
{
    route[index] = -1;

    return BitsArrSetOn(open_positions, position);
}

/********************** static for Init Next Positio Lut **********************/

/*  Init Next Positio Lut explanation:

    example 6 X 6 board, the i, j index have 6 posible moves.

                                    i ->
                                     ___ ___ ___ ___ ___ ___
                                j   |___|___|___|___|___|___|
                                    |___|___|___|_*_|___|_*_|
                                |   |___|___|_*_|___|___|___|
                                v   |___|___|___|___|i,j|___|
                                    |___|___|_*_|___|___|___|
                                    |___|___|___|_*_|___|_*_|


    the lut for this position is (36 bits):


                      (i+1, j-2)                                  (i-1, j+2)
                          v                                           v
    0 0 0 0 0 0 0 0 0 1 0 1 0 0 1 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 1
                      ^         ^                     ^                   ^
                 (i-1, j-2)   (i-2, j-1)          (i-2, j+1)        (i+1, j+2) 



    after initializing, the LUT does not change trow all the program lifetime. 


*/                    


static void InitNextPositionLUT()
{
    char i = 0, im2 = 0, im1 = 0, ip1 = 0, ip2 = 0;
    char j = 0, jm2 = 0, jm1 = 0, jp1 = 0, jp2 = 0;

    pos_ty offset = 0;

    if(ON == init_flag)
    {
        return;
    }

    for(i = 0; i < DIM; ++i)
    {
        for(j = 0; j < DIM; ++j)
        {
            offset = InitPos(j, i);

            im2 = i - 2; im1 = i - 1; ip1 = i + 1; ip2 = i + 2;
            jm2 = j - 2; jm1 = j - 1; jp1 = j + 1; jp2 = j + 2;

            if(jp2 < DIM && ip1 < DIM)
            {
                MarkToVisit(offset, jp2, ip1); 
            }
            if(jp1 < DIM && ip2 < DIM)
            {
                MarkToVisit(offset, jp1, ip2); 
            }
            if(jm1 >= 0 && ip2 < DIM)
            {
                MarkToVisit(offset, jm1, ip2); 
            }
            if(jm2 >= 0  && ip1 < DIM)
            {
                MarkToVisit(offset, jm2, ip1);  
            }
            if(jm2 >= 0  && im1 >= 0)
            {
                MarkToVisit(offset, jm2, im1); 
            }
            if(jm1 >= 0  && im2 >= 0)
            {
                MarkToVisit(offset, jm1, im2); 
            }
            if(jp1 < DIM  && im2 >= 0)
            {
                MarkToVisit(offset, jp1, im2); 
            }
            if(jp2 < DIM  && im1 >= 0)
            {
                MarkToVisit(offset, jp2, im1);
            }
        }
    }

    init_flag = ON;
}

/******************************************************************************/

static void MarkToVisit(pos_ty offset, int j, int i)
{
    pos_ty bit_position = InitPos(j, i);

    possible_position_lut[offset] = 
            BitsArrSetOn(possible_position_lut[offset], bit_position);
}

/*******************************************************************************
********************************** Print Solution *****************************/

static void PrintSolution(stat_ty status, pos_ty route[BOARD_SIZE], 
                                          pos_ty start_pos)
{
    if(FOUND == status)
    {
        BoardPrinter(route);
        printf("\nthe status for position %u in dimantion %d is SUCCESS\n\n",
                                                             start_pos, DIM);
    } 
    else if(NOT_FOUND == status)
    {
        printf("\nthe status for position %u in dimantion %d is NOT FOUND\n\n", 
                                                            start_pos, DIM);
    }
    else 
    {
        printf("\nthe status for position %u in dimantion %d is TIME OUT\n\n", 
                                                            start_pos, DIM);
    }
}

static void BoardPrinter(pos_ty route[BOARD_SIZE])
{
    int i = 0; 
    int j = 0; 

    pos_ty printer[DIM*DIM] = {0};

    for(i = 0; i < DIM; ++i)
    {
        for(j = 0; j < DIM; ++j)
        {
            printer[route[InitPos(j, i)]] = InitPos(j, i);
        }
    }

    printf("\n\n");

    for(i = 0; i < DIM; ++i)
    {
        for(j = 0; j < DIM; ++j)
        {
            printf("\033[30m\033[47m");
            printf("      "); 
            printf("\033[0m"); 
            ++j;
            printf("      "); 
        }

        printf("\n");

        for(j = 0; j < DIM; ++j)
        {
            printf("\033[30m\033[47m");
            printf("  "); 
            printf("%u", printer[InitPos(j, i)]);
            if(0 == printer[InitPos(j, i)] / 10)
            {
                printf(" ");
            }
            printf("  "); 
            printf("\033[0m"); 
            ++j;
            printf("  "); 
            printf("%u", printer[InitPos(j, i)]);
            if(0 == printer[InitPos(j, i)] / 10)
            {
                printf(" ");
            }
            printf("  ");     
        }

        printf("\n");

        for(j = 0; j < DIM; ++j)
        {
            printf("\033[30m\033[47m");
            printf("      "); 
            printf("\033[0m"); 
            ++j;
            printf("      "); 
        }

        printf("\n");
    
        ++i; 

        for(j = 0; j < DIM; ++j)
        {
            printf("\033[0m"); 
            printf("      ");
            printf("\033[30m\033[47m"); 
            ++j;
            printf("      "); 
            printf("\033[0m");
        }

        printf("\n");

        for(j = 0; j < DIM; ++j)
        {
            printf("\033[0m"); 
            printf("  "); 
            printf("%u", printer[InitPos(j, i)]);
            if(0 == printer[InitPos(j, i)] / 10)
            {
                printf(" ");
            }
            printf("  ");
            printf("\033[30m\033[47m"); 
            
            ++j;
            printf("  "); 
            printf("%u", printer[InitPos(j, i)]);
            if(0 == printer[InitPos(j, i)] / 10)
            {
                printf(" ");
            }
            printf("  "); 
            printf("\033[0m");
                
        }

        printf("\n");

        for(j = 0; j < DIM; ++j)
        {
            printf("\033[0m"); 
            printf("      "); 
            printf("\033[30m\033[47m");
            
            ++j;
            printf("      "); 
            printf("\033[0m");       
        }

        printf("\n");
    }
}
