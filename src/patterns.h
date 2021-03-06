/*=========================================================================

  Project:   Molekel

  Copyright (c) CSCS - Swiss National Supercomputing Centre.
  You may use modify and and distribute this code freely providing this
  copyright notice appears on all copies of source code and an
  acknowledgment appears with any substantial usage of the code.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

=========================================================================*/

typedef struct { unsigned edge0 : 4;
                 unsigned edge1 : 4;
		 unsigned edge2 : 4;
} tri_pattern;

/*
#*************** PATTERNS ************************************************
#
#
#               vertex - values                   edge - numbers
#
#
#               128----------64                    .-----6-----.
#                /|          /|                   /|          /|
#               / |         / |                  7 11        5 10
#              /  |        /  |                 /  |        /  |
#            16----------32   |                .-----4-----.   |
#             |   |       |   |                |   |       |   |
#             |   |       |   |                8   |       9   |
#             |   8-------|---4                |   .-----2-|---.
#             |  /        |  /                 |  /        |  /
#             | /         | /                  | 3         | 1
#             |/          |/                   |/          |/
#             1-----------2                    .-----0-----.
#
#
#
#**************************************************************************
*/
static unsigned char npat_A[] = {
0, 1, 1, 2, 1, 4, 2, 3, 1, 2, 4, 3, 2, 3, 3, 2, 1, 2, 4, 3, 6, 5, 5, 4, 4,
3, 5, 4, 5, 4, 4, 3, 1, 4, 2, 3, 4, 5, 3, 4, 6, 5, 5, 4, 5, 4, 4, 3, 2, 3,
3, 2, 5, 4, 4, 3, 5, 4, 4, 3, 4, 3, 3, 2, 1, 6, 4, 5, 2, 5, 3, 4, 4, 5, 5,
4, 3, 4, 4, 3, 4, 5, 5, 4, 5, 4, 4, 3, 5, 4, 4, 3, 4, 3, 3, 2, 2, 5, 3, 4,
3, 4, 2, 3, 5, 4, 4, 3, 4, 3, 3, 2, 3, 4, 4, 3, 4, 3, 3, 2, 4, 3, 3, 2, 3,
2, 2, 1, 1, 4, 6, 5, 4, 5, 5, 4, 2, 3, 5, 4, 3, 4, 4, 3, 2, 3, 5, 4, 5, 4,
4, 3, 3, 2, 4, 3, 4, 3, 3, 2, 4, 5, 5, 4, 5, 4, 4, 3, 5, 4, 4, 3, 4, 3, 3,
2, 3, 4, 4, 3, 4, 3, 3, 2, 4, 3, 3, 2, 3, 2, 2, 1, 2, 5, 5, 4, 3, 4, 4, 3,
3, 4, 4, 3, 2, 3, 3, 2, 3, 4, 4, 3, 4, 3, 3, 2, 4, 3, 3, 2, 3, 2, 2, 1, 3,
4, 4, 3, 4, 3, 3, 2, 4, 3, 3, 2, 3, 2, 2, 1, 2, 3, 3, 2, 3, 2, 2, 1, 3, 2,
2, 1, 2, 1, 1, 0 };

static unsigned char npat_B[] = {
0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 2, 1, 2, 2, 3, 2, 3, 3, 4, 2, 
3, 3, 4, 3, 4, 4, 3, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 3, 2, 3, 
3, 2, 3, 4, 4, 3, 3, 4, 4, 3, 4, 5, 5, 2, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3,
4, 3, 4, 4, 3, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 4, 2, 3, 3, 4, 
3, 4, 2, 3, 3, 4, 4, 5, 4, 5, 3, 2, 3, 4, 4, 3, 4, 5, 3, 2, 4, 5, 5, 4, 5,
6, 4, 1, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 3, 2, 3, 3, 4, 3, 4, 
4, 5, 3, 2, 4, 3, 4, 3, 5, 2, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5,
4, 3, 4, 4, 3, 4, 5, 5, 4, 4, 3, 5, 2, 5, 4, 6, 1, 2, 3, 3, 4, 3, 4, 4, 5, 
3, 4, 4, 5, 2, 3, 3, 2, 3, 4, 4, 5, 4, 5, 5, 6, 4, 3, 5, 4, 3, 2, 4, 1, 3, 
4, 4, 5, 4, 5, 3, 4, 4, 5, 5, 6, 3, 4, 2, 1, 2, 3, 3, 2, 3, 4, 2, 1, 3, 2,
4, 1, 2, 1, 1, 0 };

static tri_pattern __A[] = {
{ 0, 8, 3}, 
{ 0, 1, 9}, 
{ 8, 3, 1}, { 1, 9, 8}, 
{ 1, 2,10}, 
{ 8, 3, 2}, { 8, 2,10}, { 8,10, 1}, { 8, 1, 0}, 
{ 0, 2, 9}, { 2,10, 9}, 
{ 3, 2, 8}, { 2,10, 8}, {10, 9, 8}, 
{ 3,11, 2}, 
{ 0, 8, 2}, { 8,11, 2}, 
{ 3,11, 0}, { 0,11, 9}, { 9,11, 1}, { 1,11, 2}, 
{ 2, 1,11}, { 1, 9,11}, { 9, 8,11}, 
{ 3,10, 1}, { 3,11,10}, 
{ 1, 0,10}, { 0, 8,10}, { 8,11,10}, 
{ 0, 3, 9}, { 3,11, 9}, {11,10, 9}, 
{ 8,11, 9}, {11,10, 9}, 
{ 8, 4, 7}, 
{ 0, 4, 3}, { 4, 7, 3}, 
{ 0, 7, 8}, { 1, 7, 0}, { 9, 7, 1}, { 4, 7, 9}, 
{ 4, 7, 9}, { 7, 1, 9}, { 7, 3, 1}, 
{ 8, 1, 2}, {10, 1, 4}, {10, 4, 7}, {10, 7, 2}, { 7, 8, 2}, { 1, 8, 4}, 
{ 0, 4, 1}, { 1, 4,10}, {10, 4, 7}, {10, 7, 2}, { 2, 7, 3}, 
{ 0, 2, 8}, { 8, 2, 7}, { 7, 2,10}, { 7,10, 4}, { 4,10, 9}, 
{ 4, 7, 3}, { 3,10, 4}, {10, 9, 4}, { 3, 2,10}, 
{ 8, 4, 3}, { 3, 4, 2}, { 2, 4,11}, {11, 4, 7}, 
{11, 2, 7}, { 7, 2, 4}, { 2, 0, 4}, 
{ 8, 0, 3}, { 2, 1,11}, {11, 1, 7}, { 7, 1, 9}, { 7, 9, 4}, 
{ 9, 4, 7}, { 7,11, 9}, {11, 1, 9}, {11, 2, 1}, 
{ 8, 1, 3}, { 4, 1, 8}, {10, 1, 4}, { 7,10, 4}, {11,10, 7}, 
{10, 1,11}, { 0,11, 1}, { 4,11, 0}, { 7,11, 4}, 
{ 3, 0, 9}, {11, 3, 9}, {10,11, 9}, { 4, 8, 7}, 
{ 9, 4, 7}, { 9, 7,11}, {10, 9,11}, 
{ 9, 5, 4}, 
{ 9, 3, 0}, { 5, 3, 9}, { 4, 3, 5}, { 8, 3, 4}, 
{ 0, 1, 4}, { 1, 5, 4}, 
{ 8, 3, 4}, { 3, 5, 4}, { 3, 1, 5}, 
{ 1, 2, 9}, { 9, 2, 4}, { 4, 2, 5}, { 5, 2,10}, 
{ 1, 0, 9}, { 2, 8, 3}, { 2, 4, 8}, {10, 4, 2}, { 5, 4,10}, 
{ 5, 2,10}, { 5, 4, 2}, { 4, 0, 2}, 
{ 4, 8, 3}, { 3, 2, 4}, { 2, 5, 4}, { 2,10, 5}, 
{ 2, 3, 9}, { 5, 2, 9}, {11, 2, 5}, { 4,11, 5}, { 3,11, 4}, { 9, 3, 4}, 
{ 9, 2, 0}, { 5, 2, 9}, {11, 2, 5}, { 4,11, 5}, { 8,11, 4}, 
{ 1, 5, 2}, { 2, 5,11}, {11, 5, 4}, {11, 4, 3}, { 3, 4, 0}, 
{ 4, 8,11}, {11, 5, 4}, {11, 1, 5}, {11, 2, 1}, 
{ 1, 3, 9}, { 9, 3, 4}, { 4, 3,11}, { 4,11, 5}, { 5,11,10}, 
{ 1, 0,10}, { 0, 8,10}, { 8,11,10}, { 9, 5, 4}, 
{ 0, 3, 4}, {10, 4, 3}, {11,10, 3}, { 5, 4,10}, 
{10, 5, 4}, {10, 4, 8}, {11,10, 8}, 
{ 9, 5, 8}, { 5, 7, 8}, 
{ 9, 5, 0}, { 5, 3, 0}, { 5, 7, 3}, 
{ 0, 1, 8}, { 1, 7, 8}, { 1, 5, 7}, 
{ 3, 1, 5}, { 5, 7, 3}, 
{ 5, 7,10}, {10, 7, 2}, { 2, 7, 8}, { 2, 8, 1}, { 1, 8, 9}, 
{ 9, 5, 0}, { 5, 3, 0}, { 5, 7, 3}, { 1, 2,10}, 
{ 0, 2, 8}, { 7, 8, 2}, { 5, 7, 2}, {10, 5, 2}, 
{10, 5, 2}, { 2, 5, 3}, { 7, 3, 5}, 
{ 8, 9, 3}, { 3, 9, 2}, { 2, 9, 5}, { 2, 5,11}, {11, 5, 7}, 
{ 9, 5, 7}, { 7, 2, 9}, { 2, 0, 9}, { 7,11, 2}, 
{ 0, 1, 8}, { 1, 7, 8}, { 1, 5, 7}, { 3,11, 2}, 
{ 2, 1,11}, {11, 1, 7}, { 5, 7, 1}, 
{ 3,11, 1}, {11,10, 1}, { 8, 9, 7}, { 9, 5, 7}, 
{ 7,10, 5}, { 7,11,10}, { 9, 1, 0}, 
{ 7,10, 5}, { 7,11,10}, { 3, 8, 0}, 
{10, 5, 7}, {11,10, 7}, 
{10, 6, 5}, 
{ 6, 8, 3}, { 8, 6, 5}, { 8, 5, 0}, { 0, 5,10}, { 0,10, 3}, { 3,10, 6}, 
{10, 0, 1}, { 6, 0,10}, { 5, 0, 6}, { 9, 0, 5}, 
{10, 3, 1}, { 3,10, 6}, { 3, 6, 8}, { 8, 6, 5}, { 8, 5, 9}, 
{ 1, 2, 5}, { 2, 6, 5}, 
{ 0, 5, 1}, { 8, 5, 0}, { 6, 5, 8}, { 8, 3, 6}, { 6, 3, 2}, 
{ 9, 6, 5}, { 9, 0, 6}, { 0, 2, 6}, 
{ 5, 9, 8}, { 8, 6, 5}, { 8, 2, 6}, { 8, 3, 2}, 
{ 2, 3,10}, {10, 3, 5}, { 5, 3, 6}, { 6, 3,11}, 
{ 2, 0,10}, {10, 0, 5}, { 5, 0, 8}, { 5, 8, 6}, { 6, 8,11}, 
{10, 2, 1}, { 5, 9, 6}, { 6, 9,11}, {11, 9, 0}, {11, 0, 3}, 
{ 2, 1,11}, { 1, 9,11}, { 9, 8,11}, {10, 6, 5}, 
{ 3,11, 6}, { 3, 6, 5}, { 5, 1, 3}, 
{ 0, 1, 5}, { 5, 8, 0}, { 5,11, 8}, { 5, 6,11}, 
{ 9, 0, 5}, { 6, 5, 0}, { 3, 6, 0}, {11, 6, 3}, 
{11, 6, 5}, {11, 5, 9}, { 8,11, 9}, 
{ 5,10, 4}, { 4,10, 8}, { 8,10, 7}, { 7,10, 6}, 
{ 5, 0, 4}, {10, 0, 5}, { 3, 0,10}, { 6, 3,10}, { 6, 3, 7}, 
{ 5, 9, 4}, { 8, 0, 7}, { 7, 0, 1}, { 7, 1, 6}, { 6, 1,10}, 
{ 4, 7, 9}, { 7, 1, 9}, { 7, 3, 1}, {10, 6, 5}, 
{ 5, 1, 4}, { 4, 1, 8}, { 8, 1, 2}, { 2, 7, 8}, { 6, 7, 2}, 
{ 0, 4, 3}, { 4, 7, 3}, { 1, 2, 5}, { 2, 6, 5}, 
{ 6, 9, 5}, { 0, 9, 6}, { 2, 0, 6}, { 4, 8, 7}, 
{ 2, 7, 3}, { 2, 6, 7}, { 4, 5, 9}, 
{ 6, 7,11}, { 8, 4, 5}, { 8, 5, 3}, { 3, 5,10}, { 3,10, 2}, 
{11, 2, 7}, { 7, 2, 4}, { 2, 0, 4}, {10, 6, 5}, 
{ 1, 9, 0}, { 4, 7, 8}, {10, 6, 5}, {11, 2, 3}, 
{10, 2, 1}, { 4, 5, 9}, {11, 6, 7}, 
{11, 3, 6}, { 6, 3, 5}, { 1, 5, 3}, { 4, 8, 7}, 
{ 4, 1, 0}, { 4, 5, 1}, {11, 6, 7}, 
{ 3, 8, 0}, { 4, 5, 9}, {11, 6, 7}, 
{ 4, 5, 9}, {11, 6, 7}, 
{ 9, 6, 4}, { 9,10, 6}, 
{ 9,10, 0}, { 0,10, 3}, { 3,10, 6}, { 3, 6, 8}, { 8, 6, 4}, 
{10, 0, 1}, {10, 6, 0}, { 6, 4, 0}, 
{ 8, 3, 1}, { 1, 6, 8}, { 6, 4, 8}, { 1,10, 6}, 
{ 1, 4, 9}, { 1, 2, 4}, { 2, 6, 4}, 
{ 1, 4, 9}, { 1, 2, 4}, { 2, 6, 4}, { 0, 8, 3}, 
{ 4, 0, 2}, { 6, 4, 2}, 
{ 4, 8, 3}, { 4, 3, 2}, { 6, 4, 2}, 
{ 2, 9,10}, { 3, 9, 2}, { 4, 9, 3}, {11, 4, 3}, { 6, 4,11}, 
{ 0, 8, 2}, { 8,11, 2}, { 9,10, 4}, {10, 6, 4}, 
{ 0,10, 1}, { 6,10, 0}, { 4, 6, 0}, {11, 3, 2}, 
{ 8,11, 4}, {11, 6, 4}, {10, 2, 1}, 
{ 4, 9, 6}, { 1, 6, 9}, { 3, 6, 1}, {11, 6, 3}, 
{ 8,11, 4}, {11, 6, 4}, { 9, 1, 0}, 
{ 0, 3,11}, { 0,11, 6}, { 4, 0, 6}, 
{11, 4, 8}, { 6, 4,11}, 
{ 7, 8, 6}, { 6, 8,10}, { 8, 9,10}, 
{ 0, 9,10}, {10, 3, 0}, {10, 7, 3}, {10, 6, 7}, 
{10, 6, 1}, { 0, 1, 6}, { 7, 0, 6}, { 8, 0, 7}, 
{ 6, 7,10}, {10, 7, 1}, { 3, 1, 7}, 
{ 1, 2, 6}, { 6, 9, 1}, { 6, 8, 9}, { 6, 7, 8}, 
{ 2, 7, 3}, { 2, 6, 7}, { 9, 1, 0}, 
{ 6, 7, 8}, { 6, 8, 0}, { 2, 6, 0}, 
{ 7, 3, 2}, { 6, 7, 2}, 
{ 8, 7, 6}, { 8, 6,10}, { 9, 8,10}, {11, 3, 2}, 
{ 9, 2, 0}, { 9,10, 2}, {11, 6, 7}, 
{ 3, 8, 0}, {10, 2, 1}, {11, 6, 7}, 
{10, 2, 1}, { 6, 7,11}, 
{ 1, 3, 8}, { 8, 9, 1}, {11, 6, 7}, 
{ 9, 1, 0}, { 6, 7,11}, 
{ 3, 8, 0}, { 6, 7,11}, 
{ 6, 7,11}, 
{ 7, 6,11}, 
{ 3, 6,11}, { 0, 6, 3}, { 8, 6, 0}, { 7, 6, 8}, 
{ 1, 9, 6}, { 9, 6, 7}, { 0, 7, 9}, {11, 7, 0}, { 0,11, 1}, { 1,11, 6}, 
{ 3, 1,11}, {11, 1, 6}, { 6, 1, 9}, { 7, 9, 6}, { 8, 9, 7}, 
{11, 7, 2}, { 2, 7, 1}, { 1, 7,10}, {10, 7, 6}, 
{11, 3, 2}, { 1, 0, 8}, {10, 1, 8}, { 7,10, 8}, { 6,10, 7}, 
{11, 0, 2}, {11, 0, 7}, { 9, 0, 7}, { 6, 9, 7}, {10, 9, 6}, 
{ 3, 2, 8}, { 2,10, 8}, {10, 9, 8}, { 7, 6,11}, 
{ 3, 7, 2}, { 7, 6, 2}, 
{ 7, 6, 8}, { 8, 6, 0}, { 6, 2, 0}, 
{ 2, 1, 6}, { 6, 1, 9}, { 9, 7, 6}, { 0, 7, 9}, { 3, 7, 0}, 
{ 2, 1, 6}, { 9, 6, 1}, { 8, 6, 9}, { 7, 6, 8}, 
{ 7, 6,10}, { 7,10, 1}, { 1, 3, 7}, 
{ 6,10, 1}, { 1, 0, 6}, { 0, 7, 6}, { 0, 8, 7}, 
{ 9, 0,10}, { 3,10, 0}, { 7,10, 3}, { 6,10, 7}, 
{ 8, 7, 6}, { 8, 6,10}, { 9, 8,10}, 
{ 4,11, 8}, { 4, 6,11}, 
{ 3, 0,11}, {11, 0, 6}, { 0, 4, 6}, 
{ 0,11, 8}, { 1,11, 0}, { 6,11, 1}, { 9, 6, 1}, { 4, 6, 9}, 
{ 9, 4, 6}, { 6, 1, 9}, { 6, 3, 1}, { 6,11, 3}, 
{11, 8, 2}, { 2, 8, 1}, { 1, 8, 4}, { 1, 4,10}, {10, 4, 6}, 
{ 3, 0,11}, {11, 0, 6}, { 0, 4, 6}, { 1, 2,10}, 
{ 2, 9, 0}, {10, 9, 2}, { 4,11, 8}, { 4, 6,11}, 
{ 4, 6, 9}, { 6,10, 9}, { 2,11, 3}, 
{ 8, 4, 3}, { 3, 4, 2}, { 4, 6, 2}, 
{ 0, 4, 2}, { 4, 6, 2}, 
{ 4, 8, 3}, { 4, 3, 2}, { 6, 4, 2}, { 1, 0, 9}, 
{ 4, 1, 9}, { 2, 1, 4}, { 6, 2, 4}, 
{ 3, 8, 1}, { 6, 1, 8}, { 4, 6, 8}, {10, 1, 6}, 
{ 0,10, 1}, { 6,10, 0}, { 4, 6, 0}, 
{ 4, 6, 9}, { 6,10, 9}, { 3, 8, 0}, 
{ 6, 9, 4}, {10, 9, 6}, 
{ 6,11, 5}, { 5,11, 9}, { 9,11, 4}, { 4,11, 7}, 
{ 4, 8, 7}, { 3, 0, 9}, { 3, 9, 5}, { 3, 5,11}, {11, 5, 6}, 
{ 6, 1, 5}, {11, 1, 6}, { 0, 1,11}, { 7, 0,11}, { 4, 0, 7}, 
{ 8, 3, 4}, { 3, 5, 4}, { 3, 1, 5}, { 7, 6,11}, 
{10, 5, 6}, { 7, 4,11}, { 4, 9,11}, {11, 9, 2}, { 2, 9, 1}, 
{ 0, 8, 3}, { 9, 5, 4}, { 1, 2,10}, { 7, 6,11}, 
{ 2, 5,10}, { 4, 5, 2}, { 0, 4, 2}, { 6, 7,11}, 
{ 2,11, 3}, { 7, 4, 8}, { 5, 6,10}, 
{ 6, 2, 5}, { 5, 2, 9}, { 9, 2, 3}, { 9, 3, 4}, { 4, 3, 7}, 
{ 7, 6, 8}, { 8, 6, 0}, { 6, 2, 0}, { 9, 5, 4}, 
{ 3, 7, 2}, { 7, 6, 2}, { 0, 1, 4}, { 1, 5, 4}, 
{ 5, 2, 1}, { 5, 6, 2}, { 7, 4, 8}, 
{ 6, 7,10}, {10, 7, 1}, { 3, 1, 7}, { 5, 9, 4}, 
{ 9, 1, 0}, { 7, 4, 8}, { 5, 6,10}, 
{ 3, 4, 0}, { 3, 7, 4}, { 5, 6,10}, 
{ 7, 4, 8}, { 5, 6,10}, 
{ 6,11, 5}, { 5,11, 9}, {11, 8, 9}, 
{ 0, 9, 5}, { 5, 6, 0}, { 6, 3, 0}, { 6,11, 3}, 
{ 1, 0, 5}, { 8, 5, 0}, {11, 5, 8}, { 6, 5,11}, 
{11, 3, 6}, { 6, 3, 5}, { 1, 5, 3}, 
{11, 6, 5}, {11, 5, 9}, { 8,11, 9}, { 2, 1,10}, 
{ 9, 1, 0}, { 2,11, 3}, { 5, 6,10}, 
{ 2, 8, 0}, { 2,11, 8}, { 5, 6,10}, 
{ 2,11, 3}, { 5, 6,10}, 
{ 9, 5, 8}, { 6, 8, 5}, { 2, 8, 6}, { 3, 8, 2}, 
{ 6, 9, 5}, { 0, 9, 6}, { 2, 0, 6}, 
{ 5, 2, 1}, { 5, 6, 2}, { 3, 8, 0}, 
{ 2, 1, 5}, { 6, 2, 5}, 
{ 1, 3, 8}, { 8, 9, 1}, { 5, 6,10}, 
{ 9, 1, 0}, { 5, 6,10}, 
{ 3, 8, 0}, { 5, 6,10}, 
{ 6,10, 5}, 
{ 5,10, 7}, {10,11, 7}, 
{ 3,10,11}, { 0,10, 3}, { 5,10, 0}, { 0, 5, 8}, { 8, 5, 7}, 
{10,11, 1}, { 1,11, 0}, { 0,11, 7}, { 0, 7, 9}, { 9, 7, 5}, 
{ 8, 3, 1}, { 1, 9, 8}, { 5,10, 7}, {10,11, 7}, 
{ 1, 2,11}, { 1,11, 7}, { 7, 5, 1}, 
{ 2, 1,11}, {11, 1, 7}, { 5, 7, 1}, { 8, 0, 3}, 
{ 5, 9, 7}, { 2, 7, 9}, { 0, 2, 9}, {11, 7, 2}, 
{ 8, 5, 9}, { 8, 7, 5}, { 2,11, 3}, 
{ 5,10, 2}, { 5, 2, 3}, { 3, 7, 5}, 
{ 2, 0, 8}, { 8, 7, 2}, { 7, 5, 2}, { 5,10, 2}, 
{10, 5, 2}, { 2, 5, 3}, { 7, 3, 5}, { 1, 0, 9}, 
{ 8, 5, 9}, { 8, 7, 5}, {10, 2, 1}, 
{ 1, 3, 5}, { 7, 5, 3}, 
{ 1, 0, 8}, { 7, 1, 8}, { 5, 1, 7}, 
{ 5, 9, 0}, { 3, 5, 0}, { 7, 5, 3}, 
{ 5, 9, 8}, { 7, 5, 8}, 
{ 5,10, 4}, { 4,10, 8}, {10,11, 8}, 
{ 3, 0, 4}, { 4,10, 3}, {10,11, 3}, { 4, 5,10}, 
{10, 5, 4}, {10, 4, 8}, {11,10, 8}, { 1, 0, 9}, 
{ 1,10, 3}, {10,11, 3}, { 4, 5, 9}, 
{ 8, 4,11}, { 5,11, 4}, { 1,11, 5}, { 2,11, 1}, 
{ 4, 1, 0}, { 4, 5, 1}, { 2,11, 3}, 
{ 2, 8, 0}, { 2,11, 8}, { 4, 5, 9}, 
{ 2,11, 3}, { 4, 5, 9}, 
{ 8, 4, 3}, { 2, 3, 4}, { 5, 2, 4}, {10, 2, 5}, 
{ 2, 5,10}, { 4, 5, 2}, { 0, 4, 2}, 
{ 3, 8, 0}, {10, 2, 1}, { 4, 5, 9}, 
{10, 2, 1}, { 4, 5, 9}, 
{ 3, 8, 4}, { 5, 3, 4}, { 1, 3, 5}, 
{ 1, 0, 4}, { 5, 1, 4}, 
{ 3, 8, 0}, { 4, 5, 9}, 
{ 5, 9, 4}, 
{ 4, 9, 7}, { 7, 9,11}, { 9,10,11}, 
{ 4, 9, 7}, { 7, 9,11}, { 9,10,11}, { 0, 8, 3}, 
{ 1,10,11}, {11, 0, 1}, {11, 4, 0}, {11, 7, 4}, 
{ 1,10, 3}, {10,11, 3}, { 7, 4, 8}, 
{ 4, 9, 7}, {11, 7, 9}, { 1,11, 9}, { 2,11, 1}, 
{ 9, 1, 0}, { 2,11, 3}, { 7, 4, 8}, 
{ 2,11, 7}, { 2, 7, 4}, { 0, 2, 4}, 
{ 2,11, 3}, { 7, 4, 8}, 
{ 7, 4, 3}, {10, 3, 4}, { 9,10, 4}, { 2, 3,10}, 
{ 9, 2, 0}, { 9,10, 2}, { 7, 4, 8}, 
{ 3, 4, 0}, { 3, 7, 4}, {10, 2, 1}, 
{10, 2, 1}, { 7, 4, 8}, 
{ 7, 4, 9}, { 1, 7, 9}, { 3, 7, 1}, 
{ 9, 1, 0}, { 7, 4, 8}, 
{ 4, 0, 3}, { 7, 4, 3}, 
{ 4, 8, 7}, 
{11, 8, 9}, {10,11, 9}, 
{ 3, 0, 9}, {11, 3, 9}, {10,11, 9}, 
{ 0, 1,10}, { 8, 0,10}, {11, 8,10}, 
{10, 3, 1}, {11, 3,10}, 
{ 1, 2,11}, { 9, 1,11}, { 8, 9,11}, 
{ 9, 1, 0}, { 2,11, 3}, 
{ 8, 0, 2}, {11, 8, 2}, 
{11, 3, 2}, 
{ 2, 3, 8}, {10, 2, 8}, { 9,10, 8}, 
{ 2, 0, 9}, {10, 2, 9}, 
{ 3, 8, 0}, {10, 2, 1}, 
{ 2, 1,10}, 
{ 3, 8, 1}, { 9, 1, 8}, 
{ 1, 0, 9}, 
{ 8, 0, 3}
};

static tri_pattern __B[] = {
{ 0, 3, 8}, 
{ 0, 9, 1}, 
{ 8, 1, 3}, { 1, 8, 9}, 
{ 1,10, 2}, 
{ 0, 3, 8}, { 1,10, 2}, 
{ 0, 9, 2}, { 2, 9,10}, 
{ 3, 8, 2}, { 2, 8,10}, {10, 8, 9}, 
{ 3, 2,11}, 
{ 0, 2, 8}, { 8, 2,11}, 
{ 0, 9, 1}, { 3, 2,11}, 
{ 2,11, 1}, { 1,11, 9}, { 9,11, 8}, 
{ 3, 1,10}, { 3,10,11}, 
{ 1,10, 0}, { 0,10, 8}, { 8,10,11}, 
{ 0, 9, 3}, { 3, 9,11}, {11, 9,10}, 
{ 8, 9,11}, {11, 9,10}, 
{ 8, 7, 4}, 
{ 0, 3, 4}, { 4, 3, 7}, 
{ 0, 9, 1}, { 8, 7, 4}, 
{ 4, 9, 7}, { 7, 9, 1}, { 7, 1, 3}, 
{ 1,10, 2}, { 8, 7, 4}, 
{ 0, 3, 4}, { 4, 3, 7}, { 1,10, 2}, 
{ 0, 9, 2}, { 2, 9,10}, { 8, 7, 4}, 
{ 4, 3, 7}, { 3, 4,10}, {10, 4, 9}, { 3,10, 2}, 
{ 3, 2,11}, { 8, 7, 4}, 
{11, 7, 2}, { 7, 4, 2}, { 2, 4, 0}, 
{ 0, 9, 1}, { 3, 2,11}, { 8, 7, 4}, 
{ 9, 7, 4}, { 7, 9,11}, {11, 9, 1}, {11, 1, 2}, 
{ 3, 1,10}, { 3,10,11}, { 8, 7, 4}, 
{10,11, 1}, { 0, 1,11}, { 4, 0,11}, { 7, 4,11}, 
{ 3, 9, 0}, {11, 9, 3}, {10, 9,11}, { 4, 7, 8}, 
{ 9, 7, 4}, { 9,11, 7}, {10,11, 9}, 
{ 9, 4, 5}, 
{ 0, 3, 8}, { 9, 4, 5}, 
{ 0, 4, 1}, { 1, 4, 5}, 
{ 8, 4, 3}, { 3, 4, 5}, { 3, 5, 1}, 
{ 1,10, 2}, { 9, 4, 5}, 
{ 0, 3, 8}, { 1,10, 2}, { 9, 4, 5}, 
{ 5,10, 2}, { 5, 2, 4}, { 4, 2, 0}, 
{ 4, 3, 8}, { 3, 4, 2}, { 2, 4, 5}, { 2, 5,10}, 
{ 3, 2,11}, { 9, 4, 5}, 
{ 0, 2, 8}, { 8, 2,11}, { 9, 4, 5}, 
{ 0, 4, 1}, { 1, 4, 5}, { 3, 2,11}, 
{ 4,11, 8}, {11, 4, 5}, {11, 5, 1}, {11, 1, 2}, 
{ 3, 1,10}, { 3,10,11}, { 9, 4, 5}, 
{ 1,10, 0}, { 0,10, 8}, { 8,10,11}, { 9, 4, 5}, 
{ 0, 4, 3}, {10, 3, 4}, {11, 3,10}, { 5,10, 4}, 
{10, 4, 5}, {10, 8, 4}, {11, 8,10}, 
{ 9, 8, 5}, { 5, 8, 7}, 
{ 9, 0, 5}, { 5, 0, 3}, { 5, 3, 7}, 
{ 0, 8, 1}, { 1, 8, 7}, { 1, 7, 5}, 
{ 3, 5, 1}, { 5, 3, 7}, 
{ 9, 8, 5}, { 5, 8, 7}, { 1,10, 2}, 
{ 9, 0, 5}, { 5, 0, 3}, { 5, 3, 7}, { 1,10, 2}, 
{ 0, 8, 2}, { 7, 2, 8}, { 5, 2, 7}, {10, 2, 5}, 
{10, 2, 5}, { 2, 3, 5}, { 7, 5, 3}, 
{ 9, 8, 5}, { 5, 8, 7}, { 3, 2,11}, 
{ 9, 7, 5}, { 7, 9, 2}, { 2, 9, 0}, { 7, 2,11}, 
{ 0, 8, 1}, { 1, 8, 7}, { 1, 7, 5}, { 3, 2,11}, 
{ 2,11, 1}, {11, 7, 1}, { 5, 1, 7}, 
{ 3, 1,11}, {11, 1,10}, { 8, 7, 9}, { 9, 7, 5}, 
{ 1,10,11}, { 0, 1,11}, { 7, 0,11}, { 9, 0, 7}, { 5, 9, 7}, 
{11, 3,10}, { 3, 0,10}, { 0, 5,10}, { 8, 0, 5}, { 7, 8, 5}, 
{10, 7, 5}, {11, 7,10}, 
{10, 5, 6}, 
{ 0, 3, 8}, {10, 5, 6}, 
{ 0, 9, 1}, {10, 5, 6}, 
{ 8, 1, 3}, { 1, 8, 9}, {10, 5, 6}, 
{ 1, 5, 2}, { 2, 5, 6}, 
{ 1, 5, 2}, { 2, 5, 6}, { 0, 3, 8}, 
{ 9, 5, 6}, { 9, 6, 0}, { 0, 6, 2}, 
{ 5, 8, 9}, { 8, 5, 6}, { 8, 6, 2}, { 8, 2, 3}, 
{ 3, 2,11}, {10, 5, 6}, 
{ 0, 2, 8}, { 8, 2,11}, {10, 5, 6}, 
{ 0, 9, 1}, { 3, 2,11}, {10, 5, 6}, 
{ 2,11, 1}, { 1,11, 9}, { 9,11, 8}, {10, 5, 6}, 
{ 3, 6,11}, { 3, 5, 6}, { 5, 3, 1}, 
{ 0, 5, 1}, { 5, 0, 8}, { 5, 8,11}, { 5,11, 6}, 
{ 9, 5, 0}, { 6, 0, 5}, { 3, 0, 6}, {11, 3, 6}, 
{11, 5, 6}, {11, 9, 5}, { 8, 9,11}, 
{ 8, 7, 4}, {10, 5, 6}, 
{ 0, 3, 4}, { 4, 3, 7}, {10, 5, 6}, 
{ 0, 9, 1}, { 8, 7, 4}, {10, 5, 6}, 
{ 4, 9, 7}, { 7, 9, 1}, { 7, 1, 3}, {10, 5, 6}, 
{ 1, 5, 2}, { 2, 5, 6}, { 8, 7, 4}, 
{ 0, 3, 4}, { 4, 3, 7}, { 1, 5, 2}, { 2, 5, 6}, 
{ 6, 5, 9}, { 0, 6, 9}, { 2, 6, 0}, { 4, 7, 8}, 
{ 5, 6, 2}, { 9, 5, 2}, { 3, 9, 2}, { 4, 9, 3}, { 7, 4, 3}, 
{ 3, 2,11}, { 8, 7, 4}, {10, 5, 6}, 
{11, 7, 2}, { 7, 4, 2}, { 2, 4, 0}, {10, 5, 6}, 
{ 1, 0, 9}, { 4, 8, 7}, {10, 5, 6}, {11, 3, 2}, 
{ 6,10, 5}, {11, 7, 4}, {11, 4, 9}, { 2,11, 9}, { 1, 2, 9}, 
{11, 6, 3}, { 6, 5, 3}, { 1, 3, 5}, { 4, 7, 8}, 
{ 5, 6, 1}, { 6,11, 1}, {11, 0, 1}, {11, 7, 0}, { 7, 4, 0}, 
{ 7, 4, 8}, { 9, 3, 0}, { 5, 3, 9}, {11, 3, 5}, { 6,11, 5}, 
{ 5, 6,11}, { 9, 5,11}, { 4, 9,11}, { 7, 4,11}, 
{ 9, 4, 6}, { 9, 6,10}, 
{ 9, 4, 6}, { 9, 6,10}, { 0, 3, 8}, 
{10, 1, 0}, {10, 0, 6}, { 6, 0, 4}, 
{ 8, 1, 3}, { 1, 8, 6}, { 6, 8, 4}, { 1, 6,10}, 
{ 1, 9, 4}, { 1, 4, 2}, { 2, 4, 6}, 
{ 1, 9, 4}, { 1, 4, 2}, { 2, 4, 6}, { 0, 3, 8}, 
{ 4, 2, 0}, { 6, 2, 4}, 
{ 4, 3, 8}, { 4, 2, 3}, { 6, 2, 4}, 
{ 9, 4, 6}, { 9, 6,10}, { 3, 2,11}, 
{ 0, 2, 8}, { 8, 2,11}, { 9, 4,10}, {10, 4, 6}, 
{ 0, 1,10}, { 6, 0,10}, { 4, 0, 6}, {11, 2, 3}, 
{ 2,11, 8}, { 1, 2, 8}, { 4, 1, 8}, {10, 1, 4}, { 6,10, 4}, 
{ 4, 6, 9}, { 1, 9, 6}, { 3, 1, 6}, {11, 3, 6}, 
{ 8, 0,11}, { 0, 1,11}, { 1, 6,11}, { 1, 9, 6}, { 9, 4, 6}, 
{ 0,11, 3}, { 0, 6,11}, { 4, 6, 0}, 
{11, 8, 4}, { 6,11, 4}, 
{ 7, 6, 8}, { 6,10, 8}, { 8,10, 9}, 
{ 0,10, 9}, {10, 0, 3}, {10, 3, 7}, {10, 7, 6}, 
{10, 1, 6}, { 0, 6, 1}, { 7, 6, 0}, { 8, 7, 0}, 
{ 6,10, 7}, {10, 1, 7}, { 3, 7, 1}, 
{ 1, 6, 2}, { 6, 1, 9}, { 6, 9, 8}, { 6, 8, 7}, 
{ 6, 2, 1}, { 9, 6, 1}, { 6, 9, 7}, { 9, 0, 7}, { 0, 3, 7}, 
{ 6, 8, 7}, { 6, 0, 8}, { 2, 0, 6}, 
{ 7, 2, 3}, { 6, 2, 7}, 
{ 8, 6, 7}, { 8,10, 6}, { 9,10, 8}, {11, 2, 3}, 
{ 2,11, 0}, { 7,11, 0}, { 7, 9, 0}, { 7, 6, 9}, { 6,10, 9}, 
{ 2,11, 3}, { 8, 1, 0}, { 8,10, 1}, { 8, 7,10}, { 7, 6,10}, 
{ 2,11, 7}, { 1, 2, 7}, {10, 1, 7}, { 6,10, 7}, 
{11, 3, 1}, { 6,11, 1}, { 9, 6, 1}, { 6, 7, 9}, { 7, 8, 9}, 
{ 6, 1, 9}, { 7, 9, 6}, { 9, 0, 7}, { 0,11, 7}, { 1, 0,11}, { 6, 1,11}, 
{11, 3, 6}, { 3, 0, 6}, { 0, 8, 6}, { 8, 7, 6}, 
{ 6,11, 7}, 
{ 7,11, 6}, 
{ 0, 3, 8}, {11, 6, 7}, 
{ 0, 9, 1}, {11, 6, 7}, 
{ 8, 1, 3}, { 1, 8, 9}, { 7,11, 6}, 
{ 1,10, 2}, {11, 6, 7}, 
{ 0, 3, 8}, { 1,10, 2}, { 7,11, 6}, 
{ 0, 9, 2}, { 2, 9,10}, { 7,11, 6}, 
{ 3, 8, 2}, { 2, 8,10}, {10, 8, 9}, { 7,11, 6}, 
{ 3, 2, 7}, { 7, 2, 6}, 
{ 7, 8, 6}, { 8, 0, 6}, { 6, 0, 2}, 
{ 3, 2, 7}, { 7, 2, 6}, { 0, 9, 1}, 
{ 2, 6, 1}, { 9, 1, 6}, { 8, 9, 6}, { 7, 8, 6}, 
{ 7,10, 6}, { 7, 1,10}, { 1, 7, 3}, 
{ 6, 1,10}, { 1, 6, 0}, { 0, 6, 7}, { 0, 7, 8}, 
{ 9,10, 0}, { 3, 0,10}, { 7, 3,10}, { 6, 7,10}, 
{ 8, 6, 7}, { 8,10, 6}, { 9,10, 8}, 
{ 4, 8,11}, { 4,11, 6}, 
{ 3,11, 0}, {11, 6, 0}, { 0, 6, 4}, 
{ 4, 8,11}, { 4,11, 6}, { 0, 9, 1}, 
{ 9, 6, 4}, { 6, 9, 1}, { 6, 1, 3}, { 6, 3,11}, 
{ 4, 8,11}, { 4,11, 6}, { 1,10, 2}, 
{ 3,11, 0}, {11, 6, 0}, { 0, 6, 4}, { 1,10, 2}, 
{ 2, 0, 9}, {10, 2, 9}, { 4, 8,11}, { 4,11, 6}, 
{10, 2, 9}, { 2, 3, 9}, { 3, 4, 9}, { 3,11, 4}, {11, 6, 4}, 
{ 8, 3, 4}, { 3, 2, 4}, { 4, 2, 6}, 
{ 0, 2, 4}, { 4, 2, 6}, 
{ 4, 3, 8}, { 4, 2, 3}, { 6, 2, 4}, { 1, 9, 0}, 
{ 4, 9, 1}, { 2, 4, 1}, { 6, 4, 2}, 
{ 3, 1, 8}, { 6, 8, 1}, { 4, 8, 6}, {10, 6, 1}, 
{ 0, 1,10}, { 6, 0,10}, { 4, 0, 6}, 
{ 0, 9,10}, { 3, 0,10}, { 6, 3,10}, { 8, 3, 6}, { 4, 8, 6}, 
{ 6, 4, 9}, {10, 6, 9}, 
{ 9, 4, 5}, { 7,11, 6}, 
{ 0, 3, 8}, { 9, 4, 5}, { 7,11, 6}, 
{ 0, 4, 1}, { 1, 4, 5}, { 7,11, 6}, 
{ 8, 4, 3}, { 3, 4, 5}, { 3, 5, 1}, { 7,11, 6}, 
{ 1,10, 2}, { 9, 4, 5}, { 7,11, 6}, 
{ 0, 3, 8}, { 9, 4, 5}, { 1,10, 2}, { 7,11, 6}, 
{ 2,10, 5}, { 4, 2, 5}, { 0, 2, 4}, { 6,11, 7}, 
{11, 6, 7}, { 5, 8, 4}, { 3, 8, 5}, {10, 3, 5}, { 2, 3,10}, 
{ 3, 2, 7}, { 7, 2, 6}, { 9, 4, 5}, 
{ 7, 8, 6}, { 8, 0, 6}, { 6, 0, 2}, { 9, 4, 5}, 
{ 3, 2, 7}, { 7, 2, 6}, { 0, 4, 1}, { 1, 4, 5}, 
{ 4, 5, 1}, { 8, 4, 1}, { 2, 8, 1}, { 8, 2, 7}, { 2, 6, 7}, 
{ 6,10, 7}, {10, 1, 7}, { 3, 7, 1}, { 5, 4, 9}, 
{ 4, 5, 9}, { 7, 8, 0}, { 1, 7, 0}, { 6, 7, 1}, {10, 6, 1}, 
{ 4, 5, 0}, { 5,10, 0}, {10, 3, 0}, {10, 6, 3}, { 7, 6, 3}, 
{ 4, 5,10}, { 8, 4,10}, { 7, 8,10}, { 6, 7,10}, 
{ 6, 5,11}, { 5, 9,11}, {11, 9, 8}, 
{ 0, 5, 9}, { 5, 0, 6}, { 6, 0, 3}, { 6, 3,11}, 
{ 1, 5, 0}, { 8, 0, 5}, {11, 8, 5}, { 6,11, 5}, 
{11, 6, 3}, { 6, 5, 3}, { 1, 3, 5}, 
{11, 5, 6}, {11, 9, 5}, { 8, 9,11}, { 2,10, 1}, 
{ 1,10, 2}, { 6, 5, 9}, {11, 6, 9}, { 0,11, 9}, { 3,11, 0}, 
{10, 2, 0}, { 5,10, 0}, { 8, 5, 0}, { 6, 5, 8}, {11, 6, 8}, 
{10, 2, 3}, { 5,10, 3}, { 6, 5, 3}, {11, 6, 3}, 
{ 9, 8, 5}, { 6, 5, 8}, { 2, 6, 8}, { 3, 2, 8}, 
{ 6, 5, 9}, { 0, 6, 9}, { 2, 6, 0}, 
{ 1, 0, 5}, { 0, 8, 5}, { 8, 6, 5}, { 6, 8, 3}, { 2, 6, 3}, 
{ 2, 5, 1}, { 6, 5, 2}, 
{ 1,10, 3}, { 6, 3,10}, { 8, 3, 6}, { 5, 8, 6}, { 9, 8, 5}, 
{ 1,10, 0}, {10, 6, 0}, { 6, 5, 0}, { 5, 9, 0}, 
{ 3, 6, 8}, { 5, 8, 6}, { 0, 8, 5}, {10, 0, 5}, { 3, 0,10}, { 6, 3,10}, 
{ 6, 5,10}, 
{ 5, 7,10}, {10, 7,11}, 
{ 5, 7,10}, {10, 7,11}, { 0, 3, 8}, 
{ 5, 7,10}, {10, 7,11}, { 0, 9, 1}, 
{ 8, 1, 3}, { 1, 8, 9}, { 5, 7,10}, {10, 7,11}, 
{ 1,11, 2}, { 1, 7,11}, { 7, 1, 5}, 
{ 2,11, 1}, {11, 7, 1}, { 5, 1, 7}, { 8, 3, 0}, 
{ 5, 7, 9}, { 2, 9, 7}, { 0, 9, 2}, {11, 2, 7}, 
{ 3, 8, 9}, { 2, 3, 9}, { 5, 2, 9}, {11, 2, 5}, { 7,11, 5}, 
{ 5, 2,10}, { 5, 3, 2}, { 3, 5, 7}, 
{ 2, 8, 0}, { 8, 2, 7}, { 7, 2, 5}, { 5, 2,10}, 
{10, 2, 5}, { 2, 3, 5}, { 7, 5, 3}, { 1, 9, 0}, 
{10, 5, 7}, { 2,10, 7}, { 8, 2, 7}, { 1, 2, 8}, { 9, 1, 8}, 
{ 1, 5, 3}, { 7, 3, 5}, 
{ 1, 8, 0}, { 7, 8, 1}, { 5, 7, 1}, 
{ 5, 0, 9}, { 3, 0, 5}, { 7, 3, 5}, 
{ 5, 8, 9}, { 7, 8, 5}, 
{ 5, 4,10}, { 4, 8,10}, {10, 8,11}, 
{ 3, 4, 0}, { 4, 3,10}, {10, 3,11}, { 4,10, 5}, 
{10, 4, 5}, {10, 8, 4}, {11, 8,10}, { 1, 9, 0}, 
{ 9, 1, 3}, { 4, 9, 3}, {11, 4, 3}, { 5, 4,11}, {10, 5,11}, 
{ 8,11, 4}, { 5, 4,11}, { 1, 5,11}, { 2, 1,11}, 
{ 2, 1, 5}, {11, 2, 5}, { 4,11, 5}, { 3,11, 4}, { 0, 3, 4}, 
{ 0, 9, 2}, { 9, 5, 2}, { 5,11, 2}, { 5, 4,11}, { 4, 8,11}, 
{ 9, 2, 3}, { 9, 5, 2}, { 5,11, 2}, { 5, 4,11}, { 4, 3,11}, { 4, 9, 3}, 
{ 8, 3, 4}, { 2, 4, 3}, { 5, 4, 2}, {10, 5, 2}, 
{ 2,10, 5}, { 4, 2, 5}, { 0, 2, 4}, 
{ 9, 1, 0}, { 3, 2, 8}, { 8, 2, 4}, { 2,10, 4}, {10, 5, 4}, 
{ 9, 1, 2}, { 4, 9, 2}, { 5, 4, 2}, {10, 5, 2}, 
{ 3, 4, 8}, { 5, 4, 3}, { 1, 5, 3}, 
{ 1, 4, 0}, { 5, 4, 1}, 
{ 0, 9, 3}, { 9, 5, 3}, { 5, 4, 3}, { 4, 8, 3}, 
{ 5, 4, 9}, 
{ 4, 7, 9}, { 7,11, 9}, { 9,11,10}, 
{ 4, 7, 9}, { 7,11, 9}, { 9,11,10}, { 0, 3, 8}, 
{ 1,11,10}, {11, 1, 0}, {11, 0, 4}, {11, 4, 7}, 
{ 3, 8, 1}, { 8, 4, 1}, { 4,10, 1}, { 4, 7,10}, { 7,11,10}, 
{ 4, 7, 9}, {11, 9, 7}, { 1, 9,11}, { 2, 1,11}, 
{ 3, 8, 0}, {11, 2, 1}, { 7,11, 1}, { 9, 7, 1}, { 4, 7, 9}, 
{ 2, 7,11}, { 2, 4, 7}, { 0, 4, 2}, 
{ 3, 8, 4}, { 2, 3, 4}, {11, 2, 4}, { 7,11, 4}, 
{ 7, 3, 4}, {10, 4, 3}, { 9, 4,10}, { 2,10, 3}, 
{ 8, 0, 2}, { 7, 8, 2}, {10, 7, 2}, { 4, 7,10}, { 9, 4,10}, 
{ 1, 0, 4}, {10, 1, 4}, { 7,10, 4}, { 2,10, 7}, { 3, 2, 7}, 
{ 2, 8, 1}, { 4,10, 1}, { 7,10, 4}, { 2,10, 7}, { 2, 7, 8}, { 4, 1, 8}, 
{ 7, 9, 4}, { 1, 9, 7}, { 3, 1, 7}, 
{ 8, 0, 7}, { 0, 1, 7}, { 1, 9, 7}, { 9, 4, 7}, 
{ 4, 3, 0}, { 7, 3, 4}, 
{ 4, 7, 8}, 
{11, 9, 8}, {10, 9,11}, 
{ 3, 9, 0}, {11, 9, 3}, {10, 9,11}, 
{ 0,10, 1}, { 8,10, 0}, {11,10, 8}, 
{10, 1, 3}, {11,10, 3}, 
{ 1,11, 2}, { 9,11, 1}, { 8,11, 9}, 
{ 0, 3,11}, { 9, 0,11}, { 1, 9,11}, { 2, 1,11}, 
{ 8, 2, 0}, {11, 2, 8}, 
{11, 2, 3}, 
{ 2, 8, 3}, {10, 8, 2}, { 9, 8,10}, 
{ 2, 9, 0}, {10, 9, 2}, 
{ 2, 8, 3}, {10, 8, 2}, { 1, 8,10}, { 0, 8, 1}, 
{ 2,10, 1}, 
{ 3, 1, 8}, { 9, 8, 1}, 
{ 1, 9, 0}, 
{ 8, 3, 0} 
};

static tri_pattern *pat_A[] = {
__A+  0, __A+  0, __A+  1, __A+  2, __A+  4, __A+  5, __A+  9, __A+ 11, 
__A+ 14, __A+ 15, __A+ 17, __A+ 21, __A+ 24, __A+ 26, __A+ 29, __A+ 32, 
__A+ 34, __A+ 35, __A+ 37, __A+ 41, __A+ 44, __A+ 50, __A+ 55, __A+ 60, 
__A+ 64, __A+ 68, __A+ 71, __A+ 76, __A+ 80, __A+ 85, __A+ 89, __A+ 93, 
__A+ 96, __A+ 97, __A+101, __A+103, __A+106, __A+110, __A+115, __A+118, 
__A+122, __A+128, __A+133, __A+138, __A+142, __A+147, __A+151, __A+155, 
__A+158, __A+160, __A+163, __A+166, __A+168, __A+173, __A+177, __A+181, 
__A+184, __A+189, __A+193, __A+197, __A+200, __A+204, __A+207, __A+210, 
__A+212, __A+213, __A+219, __A+223, __A+228, __A+230, __A+235, __A+238, 
__A+242, __A+246, __A+251, __A+256, __A+260, __A+263, __A+267, __A+271, 
__A+274, __A+278, __A+283, __A+288, __A+292, __A+297, __A+301, __A+305, 
__A+308, __A+313, __A+317, __A+321, __A+324, __A+328, __A+331, __A+334, 
__A+336, __A+338, __A+343, __A+346, __A+350, __A+353, __A+357, __A+359, 
__A+362, __A+367, __A+371, __A+375, __A+378, __A+382, __A+385, __A+388, 
__A+390, __A+393, __A+397, __A+401, __A+404, __A+408, __A+411, __A+414, 
__A+416, __A+420, __A+423, __A+426, __A+428, __A+431, __A+433, __A+435, 
__A+436, __A+437, __A+441, __A+447, __A+452, __A+456, __A+461, __A+466, 
__A+470, __A+472, __A+475, __A+480, __A+484, __A+487, __A+491, __A+495, 
__A+498, __A+500, __A+503, __A+508, __A+512, __A+517, __A+521, __A+525, 
__A+528, __A+531, __A+533, __A+537, __A+540, __A+544, __A+547, __A+550, 
__A+552, __A+556, __A+561, __A+566, __A+570, __A+575, __A+579, __A+583, 
__A+586, __A+591, __A+595, __A+599, __A+602, __A+606, __A+609, __A+612, 
__A+614, __A+617, __A+621, __A+625, __A+628, __A+632, __A+635, __A+638, 
__A+640, __A+644, __A+647, __A+650, __A+652, __A+655, __A+657, __A+659, 
__A+660, __A+662, __A+667, __A+672, __A+676, __A+679, __A+683, __A+687, 
__A+690, __A+693, __A+697, __A+701, __A+704, __A+706, __A+709, __A+712, 
__A+714, __A+717, __A+721, __A+725, __A+728, __A+732, __A+735, __A+738, 
__A+740, __A+744, __A+747, __A+750, __A+752, __A+755, __A+757, __A+759, 
__A+760, __A+763, __A+767, __A+771, __A+774, __A+778, __A+781, __A+784, 
__A+786, __A+790, __A+793, __A+796, __A+798, __A+801, __A+803, __A+805, 
__A+806, __A+808, __A+811, __A+814, __A+816, __A+819, __A+821, __A+823, 
__A+824, __A+827, __A+829, __A+831, __A+832, __A+834, __A+835, __A+836 
};

static tri_pattern *pat_B[] = {
__B+  0, __B+  0, __B+  1, __B+  2, __B+  4, __B+  5, __B+  7, __B+  9, 
__B+ 12, __B+ 13, __B+ 15, __B+ 17, __B+ 20, __B+ 22, __B+ 25, __B+ 28, 
__B+ 30, __B+ 31, __B+ 33, __B+ 35, __B+ 38, __B+ 40, __B+ 43, __B+ 46, 
__B+ 50, __B+ 52, __B+ 55, __B+ 58, __B+ 62, __B+ 65, __B+ 69, __B+ 73, 
__B+ 76, __B+ 77, __B+ 79, __B+ 81, __B+ 84, __B+ 86, __B+ 89, __B+ 92, 
__B+ 96, __B+ 98, __B+101, __B+104, __B+108, __B+111, __B+115, __B+119, 
__B+122, __B+124, __B+127, __B+130, __B+132, __B+135, __B+139, __B+143, 
__B+146, __B+149, __B+153, __B+157, __B+160, __B+164, __B+169, __B+174, 
__B+176, __B+177, __B+179, __B+181, __B+184, __B+186, __B+189, __B+192, 
__B+196, __B+198, __B+201, __B+204, __B+208, __B+211, __B+215, __B+219, 
__B+222, __B+224, __B+227, __B+230, __B+234, __B+237, __B+241, __B+245, 
__B+250, __B+253, __B+257, __B+261, __B+266, __B+270, __B+275, __B+280, 
__B+284, __B+286, __B+289, __B+292, __B+296, __B+299, __B+303, __B+305, 
__B+308, __B+311, __B+315, __B+319, __B+324, __B+328, __B+333, __B+336, 
__B+338, __B+341, __B+345, __B+349, __B+352, __B+356, __B+361, __B+364, 
__B+366, __B+370, __B+375, __B+380, __B+384, __B+389, __B+395, __B+399, 
__B+400, __B+401, __B+403, __B+405, __B+408, __B+410, __B+413, __B+416, 
__B+420, __B+422, __B+425, __B+428, __B+432, __B+435, __B+439, __B+443, 
__B+446, __B+448, __B+451, __B+454, __B+458, __B+461, __B+465, __B+469, 
__B+474, __B+477, __B+479, __B+483, __B+486, __B+490, __B+493, __B+498, 
__B+500, __B+502, __B+505, __B+508, __B+512, __B+515, __B+519, __B+523, 
__B+528, __B+531, __B+535, __B+539, __B+544, __B+548, __B+553, __B+558, 
__B+562, __B+565, __B+569, __B+573, __B+576, __B+580, __B+585, __B+590, 
__B+594, __B+598, __B+601, __B+606, __B+608, __B+613, __B+617, __B+623, 
__B+624, __B+626, __B+629, __B+632, __B+636, __B+639, __B+643, __B+647, 
__B+652, __B+655, __B+659, __B+663, __B+668, __B+670, __B+673, __B+676, 
__B+678, __B+681, __B+685, __B+689, __B+694, __B+698, __B+703, __B+708, 
__B+714, __B+718, __B+721, __B+726, __B+730, __B+733, __B+735, __B+739, 
__B+740, __B+743, __B+747, __B+751, __B+756, __B+760, __B+765, __B+768, 
__B+772, __B+776, __B+781, __B+786, __B+792, __B+795, __B+799, __B+801, 
__B+802, __B+804, __B+807, __B+810, __B+812, __B+815, __B+819, __B+821, 
__B+822, __B+825, __B+827, __B+831, __B+832, __B+834, __B+835, __B+836 
};

