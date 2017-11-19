/***************************************************************************
                          SDLMappy.cpp  -  description
                             -------------------
    begin                : Thu Aug 24 2000
    copyright            : (C) 2000 by Regis Quercioli
    email                : edorul@caramail.com
 ***************************************************************************/

/* Update 11/3/2006 R Burrows www.tilemap.co.uk
 * Changed to support block properties (FMP requirement)
 * BLKSTR gfx offsets converted to units (MapRelocate)
 * Added MapDrawFG (Draw first FG field)
 * Fixed MapGetBlock for anim blocks (untested)
 * 8bit maps can now load, converted to 24bit
 *
 * Unfixed:
 * old codebase, endian/structsize problems
 * Sets colourkey every call to MapDraw
 * colourkey, modify define below for block with transp colour topleft
 */

#define TRANSPBLOCK 0

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

////////////////////////////////////////////////////////////////////////////////////
//
// SDLMappy
//
// (c) based on on sources of Robin Burrows / rburrows@bigfoot.com.
// Mappy homepage. The current Homepage for Mappy is:
// http://www.geocities.com/SiliconValley/Vista/7336/robmpy.htm
//
// Ported to SDL and modified by Regis Quercioli / edorul@caramail.com
//
//

#include "SDLMappy.h"
#include <cstring>

// ---------------------------------------------------------------------------------
// destructor of this class, inits all members
// ---------------------------------------------------------------------------------
SDLMappy::~SDLMappy( )
{
//    SAFEDELETE( ParallaxFilename );
//    SAFEDELETE( ParallaxSurface );
	map_free_mem( );
}

// ---------------------------------------------------------------------------------
// constructor of this class, inits all members
// ---------------------------------------------------------------------------------
SDLMappy::SDLMappy(): MBSW{0}, MBSH{0}, PARAW{0}, PARAH{0}, ANDTSize{0}, colorkey{0}, ParallaxSurface{nullptr},
                      ParallaxFilename{nullptr}
{
	int i;

	MMOX = 0;
	MMOY = 0;

	MSCRW = 320;
	MSCRH = 200;

	maperror = 0;
	mapwidth = 0;
	mapheight = 0;
	mapblockwidth = 0;
	mapblockheight = 0;
	mapdepth = 0;

	mapblockstrsize = 0;
	mapnumblockstr = 0;
	mapnumblockgfx = 0;

	mapfilept = nullptr;
	mappt = nullptr;
	//	maparraypt		= nullptr;
	mapcmappt = nullptr;
	mapblockgfxpt = nullptr;
	mapblockstrpt = nullptr;
	mapanimstrpt = nullptr;
	mapanimstrendpt = nullptr;

	for (i = 0; i < 1024; i++)
		maplpDDSTiles[ i ] = nullptr;

	for (i = 0; i < 8; i++)
	{
		mapmappt[ i ] = nullptr;
		//		mapmaparraypt[ i ] = nullptr;
	}

	XPosition = YPosition = 0;

	//	ParallaxSurface  = nullptr;
	//	ScreenObject     = nullptr;
	//	ParallaxFilename = nullptr;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
void SDLMappy::map_free_mem ()
{
int i;
	for (i=0;i<8;i++) { if (mapmappt[i]!= nullptr) { free (mapmappt[i]); mapmappt[i] = nullptr; } }
	mappt = nullptr;
//	for (i=0;i<8;i++) { if (mapmaparraypt[i]!=nullptr) { free (mapmaparraypt[i]); mapmaparraypt[i] = nullptr; } }
//	maparraypt = nullptr;
	if (mapcmappt!=nullptr) { free (mapcmappt); mapcmappt = nullptr; }
	if (mapblockgfxpt!=nullptr) { free (mapblockgfxpt); mapblockgfxpt = nullptr; }
	if (mapblockstrpt!=nullptr) { free (mapblockstrpt); mapblockstrpt = nullptr; }
	if (mapanimstrpt!=nullptr) { free (mapanimstrpt); mapanimstrpt = nullptr; }

	for (i=0;(maplpDDSTiles[i]!=nullptr);i++)
		SDL_FreeSurface(maplpDDSTiles[i]);
}

// ---------------------------------------------------------------------------------
// loads a map, left and top are by default 0
// bottom and right are by default -1 which means screen size
// ---------------------------------------------------------------------------------
int SDLMappy::load_map( /*SDL_Surface * Screen ,*/ char * Filename ,
					   int left , int top , int width , int height )
{
	MMOX = left;
	MMOY = top;

	MSCRW = width;
	MSCRH = height;

	XPosition = YPosition = 0;

//	ScreenObject = Screen;

	// load the map
	auto result = map_load( Filename /*, Screen->GetDD()*/ );

    // if the map loaded ok, proceed, else return error code
	if( result != -1 )
	{
        // RB
        // Since the draw routines will end the application if you try to draw
        // outside the map area, the width and height must be =< the map
        if( ( mapwidth * mapblockwidth ) < MSCRW )
            MSCRW = ( mapwidth * mapblockwidth );

        if( ( mapheight * mapblockheight ) < MSCRH )
            MSCRH = ( mapheight * mapblockheight );

        // Also, the drawn area must be at least 3*3 tiles big
        if( MSCRW < ( mapblockwidth * 3 ) )
            MSCRW = mapblockwidth * 3;

        if( MSCRH < ( mapblockheight * 3 ) )
            MSCRH = mapblockheight * 3;


		//set the palette
//		Screen->SetPalette( 0, 256, mappept );

		MBSW = mapblockwidth;
		MBSH = mapblockheight;
	}

	return result;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_relocate()
{
	int i, deplct;
	
	for(i=0; i<mapnumblockstr; i++) {
		reinterpret_cast<BLKSTR *>(mapblockstrpt)[i].bgoff /= (mapblockwidth*mapblockheight*((mapdepth+1)/8));
		reinterpret_cast<BLKSTR *>(mapblockstrpt)[i].fgoff /= (mapblockwidth*mapblockheight*((mapdepth+1)/8));
		reinterpret_cast<BLKSTR *>(mapblockstrpt)[i].fgoff2 /= (mapblockwidth*mapblockheight*((mapdepth+1)/8));
		reinterpret_cast<BLKSTR *>(mapblockstrpt)[i].fgoff3 /= (mapblockwidth*mapblockheight*((mapdepth+1)/8));
//		printf ("B%d: %d,%d,%d,%d\n", i, ((BLKSTR *) mapblockstrpt)[i].bgoff,((BLKSTR *) mapblockstrpt)[i].fgoff,((BLKSTR *) mapblockstrpt)[i].fgoff2 ,((BLKSTR *) mapblockstrpt)[i].fgoff3 );
	}

	if (mapdepth == 8) mapdepth = 24;

	switch(mapdepth)
	{
		case 8:
			deplct=mapblockwidth*mapblockheight;
			for(i=0; i<mapnumblockgfx; i++)
			{
				maplpDDSTiles[i] = SDL_CreateRGBSurfaceFrom (mapblockgfxpt+(deplct)*i,
						mapblockwidth, mapblockheight, mapdepth, mapblockwidth, 0, 0, 0, 0);			
				//SDL_SetColors(maplpDDSTiles[i], mappept, 0, 256);
			}
			break;
		case 16:
			deplct=mapblockwidth*mapblockheight*2;
			for(i=0; i<mapnumblockgfx; i++)
				maplpDDSTiles[i] = SDL_CreateRGBSurfaceFrom (mapblockgfxpt+(deplct)*i,
						mapblockwidth, mapblockheight, mapdepth, mapblockwidth*2, 0XF800, 0X7E0, 0x1F, 0);			
			break;
		case 24:
			deplct=mapblockwidth*mapblockheight*3;
			for(i=0; i<mapnumblockgfx; i++)
				maplpDDSTiles[i] = SDL_CreateRGBSurfaceFrom (mapblockgfxpt+(deplct)*i,
					mapblockwidth, mapblockheight, mapdepth, mapblockwidth*3, 0XFF0000, 0XFF00, 0xFF, 0);			
			break;
		case 32:
			deplct=mapblockwidth*mapblockheight*4;
			for(i=0; i<mapnumblockgfx; i++)
				maplpDDSTiles[i] = SDL_CreateRGBSurfaceFrom (mapblockgfxpt+(deplct)*i,
					mapblockwidth, mapblockheight, mapdepth, mapblockwidth*4, 0XFF0000, 0XFF00, 0xFF, 0);			
			break;
		default:
			return -1;
	}

	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_load (char * mapname/*, RBDIRECTDRAW lpDD*/)
{
	map_free_mem ();
	maperror = 0;

	fopen_s(&mapfilept, mapname, "rb");
	if (mapfilept==nullptr) { maperror = MER_NOOPEN; return -1; }
	fseek (mapfilept, 0, SEEK_END);
	auto mapfilesize = ftell (mapfilept);
	fseek (mapfilept, 0, SEEK_SET);
	if (fread (&mapgenheader, 1, sizeof(GENHEAD), mapfilept)!=sizeof(GENHEAD))
	{ maperror = MER_MAPLOADERROR; fclose (mapfilept); return -1; }

	if (mapgenheader.id1!='F') maperror = MER_MAPLOADERROR;
	if (mapgenheader.id2!='O') maperror = MER_MAPLOADERROR;
	if (mapgenheader.id3!='R') maperror = MER_MAPLOADERROR;
	if (mapgenheader.id4!='M') maperror = MER_MAPLOADERROR;

	if (mapfilesize==-1) maperror = MER_MAPLOADERROR;
	if (mapfilesize!=static_cast<long int>(mapbyteswapl(mapgenheader.headsize) + 8)) maperror = MER_MAPLOADERROR;

	if (maperror) { fclose (mapfilept); return -1; }

	if (fread (&mapgenheader, 1, sizeof(GENHEAD)-4,mapfilept)!=sizeof(GENHEAD)-4)
	{ maperror = MER_MAPLOADERROR; fclose (mapfilept); return -1; }
	maperror=MER_MAPLOADERROR;
	if (mapgenheader.id1=='F') { if (mapgenheader.id2=='M') { if (mapgenheader.id3=='A')
	{ if (mapgenheader.id4=='P') maperror=MER_NONE; } } }
	if (maperror) { fclose (mapfilept); return -1; }

	while (mapfilesize!=(ftell(mapfilept)))
	{
		if (fread (&mapgenheader, 1, sizeof(GENHEAD),mapfilept)!=sizeof(GENHEAD))
		{ maperror = MER_MAPLOADERROR; fclose (mapfilept); return -1; }

		auto i = 0;
		
		if (mapgenheader.id1=='M')  if (mapgenheader.id2=='P')  if (mapgenheader.id3=='H')
			if (mapgenheader.id4=='D')
				{map_decode_mphd (); i = 1;}
		
		if (mapgenheader.id1=='C')  if (mapgenheader.id2=='M')  if (mapgenheader.id3=='A')
			if (mapgenheader.id4=='P')
				{ map_decode_cmap (); i = 1; }
		
		if (mapgenheader.id1=='B')  if (mapgenheader.id2=='K')  if (mapgenheader.id3=='D')
			if (mapgenheader.id4=='T')
				{ map_decode_bkdt (); i = 1; }
		
		if (mapgenheader.id1=='A')  if (mapgenheader.id2=='N')  if (mapgenheader.id3=='D')
			if (mapgenheader.id4=='T')
				{ map_decode_andt (); i = 1; }
		
		if (mapgenheader.id1=='B')  if (mapgenheader.id2=='G')  if (mapgenheader.id3=='F')
			if (mapgenheader.id4=='X')
				{ map_decode_bgfx (); i = 1; }
		
		if (mapgenheader.id1=='B')  if (mapgenheader.id2=='O')  if (mapgenheader.id3=='D')
			if (mapgenheader.id4=='Y')
				{ map_decode_body (); i = 1; }
		
		if (mapgenheader.id1=='L')  if (mapgenheader.id2=='Y')  if (mapgenheader.id3=='R')
			if (mapgenheader.id4=='1')
				{ map_decode_lyr1 (); i = 1; }
		
		if (mapgenheader.id1=='L')  if (mapgenheader.id2=='Y')  if (mapgenheader.id3=='R')
			if (mapgenheader.id4=='2')
				{ map_decode_lyr2 (); i = 1; }
		
		if (mapgenheader.id1=='L')  if (mapgenheader.id2=='Y')  if (mapgenheader.id3=='R')
			if (mapgenheader.id4=='3')
				{ map_decode_lyr3 (); i = 1; }
		
		if (mapgenheader.id1=='L')  if (mapgenheader.id2=='Y')  if (mapgenheader.id3=='R')
			if (mapgenheader.id4=='4')
				{ map_decode_lyr4 (); i = 1; }
		
		if (mapgenheader.id1=='L')  if (mapgenheader.id2=='Y')  if (mapgenheader.id3=='R')
			if (mapgenheader.id4=='5')
				{ map_decode_lyr5 (); i = 1; }
		
		if (mapgenheader.id1=='L')  if (mapgenheader.id2=='Y')  if (mapgenheader.id3=='R')
			if (mapgenheader.id4=='6')
				{ map_decode_lyr6 (); i = 1; }
		
		if (mapgenheader.id1=='L')  if (mapgenheader.id2=='Y')  if (mapgenheader.id3=='R')
			if (mapgenheader.id4=='7')
				{ map_decode_lyr7 (); i = 1; }
		
		if (!i) map_decode_null ();
		if (maperror) { fclose (mapfilept); return -1; }
	}
	fclose (mapfilept);

	return (map_relocate ());
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
unsigned long int SDLMappy::mapbyteswapl (unsigned long int i)
{
	auto j = i&0xFF;
	j <<= 8;
	i >>= 8;
	j  |= i&0xFF;
	j <<= 8;
	i >>= 8;
	j  |= i&0xFF;
	j <<= 8;
	i >>= 8;
	j  |= i&0xFF;
	
	return j;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_decode_null ()
{
	auto mynllpt = static_cast<char *>(malloc(mapbyteswapl(mapgenheader.headsize)));
	if (mynllpt==nullptr) { maperror = MER_OUTOFMEM; return -1; }
	fread (mynllpt, mapbyteswapl(mapgenheader.headsize), 1, mapfilept);
	free (mynllpt);

	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_decode_mphd ()
{
	auto hdrmempt = static_cast<MPHD *>(malloc(mapbyteswapl(mapgenheader.headsize)));
	if (hdrmempt==nullptr) return 0;
	fread (hdrmempt, mapbyteswapl(mapgenheader.headsize), 1, mapfilept);
	mapwidth=hdrmempt->mapwidth;
	mapheight=hdrmempt->mapheight;
	mapblockwidth=hdrmempt->blockwidth;
	mapblockheight=hdrmempt->blockheight;
	mapdepth=hdrmempt->blockdepth;
	mapblockstrsize=hdrmempt->blockstrsize;
	mapnumblockstr=hdrmempt->numblockstr;
	mapnumblockgfx=hdrmempt->numblockgfx;
	free (hdrmempt);

	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_decode_body ()
{
	mappt = static_cast<short int *>(malloc(mapbyteswapl(mapgenheader.headsize)));
	if (mappt==nullptr) { maperror = MER_OUTOFMEM; return -1; }
	fread (mappt, mapbyteswapl(mapgenheader.headsize), 1, mapfilept);
	mapmappt[0] = mappt;

	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_decode_cmap ()
{
	mapcmappt = static_cast<unsigned char *>(malloc(mapbyteswapl(mapgenheader.headsize)));
	if (mapcmappt==nullptr) { maperror = MER_OUTOFMEM; return -1; }
	fread (mapcmappt, mapbyteswapl(mapgenheader.headsize), 1, mapfilept);
	map_cma_pto_pe (mapcmappt, mappept);

	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_decode_bkdt ()
{
	mapblockstrpt = static_cast<char *>(malloc(mapbyteswapl(mapgenheader.headsize)));
	if (mapblockstrpt==nullptr) { maperror = MER_OUTOFMEM; return -1; }
	fread (mapblockstrpt, mapbyteswapl(mapgenheader.headsize), 1, mapfilept);

	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_decode_andt ()
{
	ANDTSize =  mapbyteswapl(mapgenheader.headsize);
	mapanimstrpt = static_cast<char *>(malloc(mapbyteswapl(mapgenheader.headsize)));
	if (mapanimstrpt==nullptr) { maperror = MER_OUTOFMEM; return -1; }
	mapanimstrendpt = static_cast<char *>(mapanimstrpt + (mapbyteswapl(mapgenheader.headsize)));
	fread (mapanimstrpt, mapbyteswapl(mapgenheader.headsize), 1, mapfilept);

	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_decode_bgfx ()
{
	unsigned long int i;
	unsigned long int temp;

	auto size = mapbyteswapl(mapgenheader.headsize);
	if (mapdepth == 8) size *= 3;	//convert 8bit to 24bit
	mapblockgfxpt = static_cast<char *>(malloc(size));
	if (mapblockgfxpt==nullptr) { maperror = MER_OUTOFMEM; return -1; }
	switch(mapdepth)
	{
		case 8:
//			printf("maps en 8 bits non supportees pour l'instant...\n");
//			return -1;
//			fread (mapblockgfxpt, size, 1, mapfilept);
			for (i=0; i<size; i+=3)
			{
				fread (&temp, 1, 1, mapfilept);
				mapblockgfxpt[i]=mapcmappt[(temp&0XFF)*3+2];
				mapblockgfxpt[i+1]=mapcmappt[(temp&0XFF)*3+1];
				mapblockgfxpt[i+2]=mapcmappt[(temp&0XFF)*3];
			}
			break;
		case 16:
			for (i=0; i<size; i+=2)
			{
				fread (&temp, 2, 1, mapfilept);
				mapblockgfxpt[i+1]=temp&0XFF;
				temp>>=8;
				mapblockgfxpt[i]=temp&0XFF;
			}
			break;
		case 24:
			for (i=0; i<size; i+=3)
			{
				fread (&temp, 3, 1, mapfilept);
				mapblockgfxpt[i+2]=temp&0XFF;
				temp>>=8;
				mapblockgfxpt[i+1]=temp&0XFF;
				temp>>=8;
				mapblockgfxpt[i]=temp&0XFF;
			}
			break;
		case 32:
			for (i=0; i<size; i+=4)
			{
				fread (&temp, 4, 1, mapfilept);
				mapblockgfxpt[i+3]=temp&0XFF;
				temp>>=8;
				mapblockgfxpt[i+2]=temp&0XFF;
				temp>>=8;
				mapblockgfxpt[i+1]=temp&0XFF;
				temp>>=8;
				mapblockgfxpt[i]=temp&0XFF;
			}
			break;
		default:
			return -1;
	}

	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_decode_lyr1 ()
{
	mapmappt[1] = static_cast<short int *>(malloc(mapbyteswapl(mapgenheader.headsize)));
	if (mapmappt[1]==nullptr) { maperror = MER_OUTOFMEM; return -1; }
	fread (mapmappt[1], mapbyteswapl(mapgenheader.headsize), 1, mapfilept);

	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_decode_lyr2 ()
{
	mapmappt[2] = static_cast<short int *>(malloc(mapbyteswapl(mapgenheader.headsize)));
	if (mapmappt[2]==nullptr) { maperror = MER_OUTOFMEM; return -1; }
	fread (mapmappt[2], mapbyteswapl(mapgenheader.headsize), 1, mapfilept);

	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_decode_lyr3 ()
{
	mapmappt[3] = static_cast<short int *>(malloc(mapbyteswapl(mapgenheader.headsize)));
	if (mapmappt[3]==nullptr) { maperror = MER_OUTOFMEM; return -1; }
	fread (mapmappt[3], mapbyteswapl(mapgenheader.headsize), 1, mapfilept);

	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_decode_lyr4 ()
{
	mapmappt[4] = static_cast<short int *>(malloc(mapbyteswapl(mapgenheader.headsize)));
	if (mapmappt[4]==nullptr) { maperror = MER_OUTOFMEM; return -1; }
	fread (mapmappt[4], mapbyteswapl(mapgenheader.headsize), 1, mapfilept);

	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_decode_lyr5 ()
{
	mapmappt[5] = static_cast<short int *>(malloc(mapbyteswapl(mapgenheader.headsize)));
	if (mapmappt[5]==nullptr) { maperror = MER_OUTOFMEM; return -1; }
	fread (mapmappt[5], mapbyteswapl(mapgenheader.headsize), 1, mapfilept);

	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_decode_lyr6 ()
{
	mapmappt[6] = static_cast<short int *>(malloc(mapbyteswapl(mapgenheader.headsize)));
	if (mapmappt[6]==nullptr) { maperror = MER_OUTOFMEM; return -1; }
	fread (mapmappt[6], mapbyteswapl(mapgenheader.headsize), 1, mapfilept);

	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
int SDLMappy::map_decode_lyr7 ()
{
	mapmappt[7] = static_cast<short int *>(malloc(mapbyteswapl(mapgenheader.headsize)));
	if (mapmappt[7]==nullptr) { maperror = MER_OUTOFMEM; return -1; }
	fread (mapmappt[7], mapbyteswapl(mapgenheader.headsize), 1, mapfilept);

	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
void SDLMappy::map_cma_pto_pe (unsigned char * mycmappt, SDL_Color * mypept)
{
	for (auto i = 0;i<256;i++)
	{
		mypept[i].r = static_cast<char>(*mycmappt); mycmappt++;
		mypept[i].g = static_cast<char>(*mycmappt); mycmappt++;
		mypept[i].b = static_cast<char>(*mycmappt); mycmappt++;
		//mypept[i].unused = 0;
	}
}

// ---------------------------------------------------------------------------------
// Returns a BLKSTR pointer, useful for collision detection and examining a block structure.
// ---------------------------------------------------------------------------------
BLKSTR * SDLMappy::map_get_block (int x, int y) const
{
	short int * mymappt;

	/*	if (maparraypt!= nullptr) {
		mymappt = maparraypt[y]+x;
	} else */{
		mymappt = mappt;
		mymappt += x;
		mymappt += y*mapwidth;
	}
	if (*mymappt>=0) return reinterpret_cast<BLKSTR*>(static_cast<char *>(mapblockstrpt) + *mymappt);
	auto myanpt = reinterpret_cast<ANISTR *>(mapanimstrendpt + *mymappt);
	return reinterpret_cast<BLKSTR *>(static_cast<char *>(mapblockstrpt) + *reinterpret_cast<long int *>(myanpt->ancuroff));
}

// ---------------------------------------------------------------------------------
// The x and y paramaters are the offset from the left and top of the map in BLOCKS, NOT pixels.
// ---------------------------------------------------------------------------------
void SDLMappy::map_set_block (int x, int y, int strvalue) const
{
	short int * mymappt;

/*	if (maparraypt!= nullptr) {
		mymappt = maparraypt[y]+x;
	} else */{
		mymappt = mappt;
		mymappt += x;
		mymappt += y*mapwidth;
	}
	if (strvalue>=0) *mymappt = strvalue*sizeof(BLKSTR);
	else *mymappt = strvalue*sizeof(ANISTR);
}

// ------------------------------------------------------------
// renvoie le numero de la tile a l'emplacement (x,y) en blocks
//
// returns the content of a block
// if return is positive then the block is a normal tile
// if return is negative then the block (-X) is animation (+X)
// ------------------------------------------------------------
short int SDLMappy::map_get_tile( int x , int y ) const
{
	auto content = mappt[x+(y*mapwidth)];

	if( content >= 0 )
		content /= sizeof(BLKSTR);
	else
	{
		content = -content / sizeof(ANISTR) ;
		content = -content + 1;
	}

	return content;
}

// ---------------------------------------------------------------------------------
// returns the width of the map in pixels
// ---------------------------------------------------------------------------------
short int SDLMappy::get_map_width() const
{
	return mapwidth * mapblockwidth;
}



// ---------------------------------------------------------------------------------
// returns the width of the map in pixels
// ---------------------------------------------------------------------------------
short int SDLMappy::get_map_height() const
{
	return mapheight * mapblockheight;
}



// ---------------------------------------------------------------------------------
// returns the width of the map in blocks
// ---------------------------------------------------------------------------------
short int SDLMappy::get_map_width_in_blocks() const
{
	return mapwidth;
}



// ---------------------------------------------------------------------------------
// returns the width of the map in blocks
// ---------------------------------------------------------------------------------
short int SDLMappy::get_map_height_in_blocks() const
{
	return mapheight;
}



// ---------------------------------------------------------------------------------
// returns the width of one map block in pixels
// ---------------------------------------------------------------------------------
short int SDLMappy::get_map_block_width() const
{
	return mapblockwidth;
}



// ---------------------------------------------------------------------------------
// returns the height of one map block in pixels
// ---------------------------------------------------------------------------------
short int SDLMappy::get_map_block_height() const
{
	return mapblockheight;
}

// ---------------------------------------------------------------------------------
// returns the color depth of the map
// ---------------------------------------------------------------------------------
short int SDLMappy::get_map_bpp() const
{
    return mapdepth;
}

// ---------------------------------------------------------------------------------
// loads a map, left and top are by default 0
// bottom and right are by default -1 which means screen size
// ---------------------------------------------------------------------------------
void SDLMappy::map_move_to( int x , int y )
{
	XPosition = x;
    YPosition = y;

    int width = get_map_width( );

    if( XPosition > (width - 1 - MSCRW))
        XPosition = width-1-MSCRW;

    if( XPosition < 0 )
        XPosition = 0;

    int height = get_map_height( );

    if( YPosition > (height - 1 - MSCRH))
        YPosition = height-1-MSCRH;

    if( YPosition < 0 )
        YPosition = 0;
}

// ---------------------------------------------------------------------------------
// change le numero de la layer (0 à 7) devant etre affichee
// ---------------------------------------------------------------------------------
int SDLMappy::map_change_layer (int newlyr)
{
	if (newlyr<0 || newlyr>7 || mapmappt[newlyr] == nullptr) return -1;
	mappt = mapmappt[newlyr]; //maparraypt = mapmaparraypt[newlyr];
	return newlyr;
}

// ---------------------------------------------------------------------------------
// draws the background layer without Transparency
// ---------------------------------------------------------------------------------
int SDLMappy::map_draw_bg (SDL_Surface * lp_dds )
{
	int			i;
	SDL_Rect tile_dest_rect;
	
	for(i=0; i<mapnumblockgfx; i++)
		SDL_SetColorKey(maplpDDSTiles[i], 0, 0);

	auto mapxo = XPosition/mapblockwidth;
	auto mapyo = YPosition/mapblockheight;
	auto maphclip = XPosition%mapblockwidth;
	auto mapvclip = YPosition%mapblockheight;	
	
	for(i=0; i<lp_dds->h/mapblockheight+2;i++)
		for(auto j = 0; j<lp_dds->w/mapblockwidth+2;j++)
		{	
			tile_dest_rect.y	= i*mapblockheight-mapvclip;
			tile_dest_rect.x	= j*mapblockwidth-maphclip;
			tile_dest_rect.h	= mapblockheight;
			tile_dest_rect.w	= mapblockwidth;
			
			int numtile = mappt[mapxo+j+((mapyo+i)*mapwidth)];
			
			if (((mapxo+j)<mapwidth) && ((mapyo+i)<mapheight))
			    if (numtile>=0)
				SDL_BlitSurface(maplpDDSTiles[reinterpret_cast<BLKSTR *>(mapblockstrpt + numtile)->bgoff],
					nullptr, lp_dds, &tile_dest_rect);
				else
				{
					auto anim = reinterpret_cast<ANISTR *>(mapanimstrendpt + numtile);
					auto numanim = mapanimstrpt[ANDTSize+anim->ancuroff+1]&0XFF;
					numanim <<= 8;
					numanim |= mapanimstrpt[ANDTSize+anim->ancuroff]&0XFF;
//					numanim >>= 5;
					SDL_BlitSurface(maplpDDSTiles[reinterpret_cast<BLKSTR *>(mapblockstrpt + numanim)->bgoff],
						nullptr, lp_dds, &tile_dest_rect);
				}
		}
	return 0;
}

// ---------------------------------------------------------------------------------
// draws the background layer WITH Transparency (couleur de transparence = celle du
//			1er block qui doit etre uni)
// ---------------------------------------------------------------------------------
int SDLMappy::map_draw_bgt (SDL_Surface * lp_dds )
{
	int			i;
	SDL_Rect tile_dest_rect;
	
	switch(mapdepth)
	{
		case 8:
			for(i=0; i<mapnumblockgfx; i++)
				SDL_SetColorKey(maplpDDSTiles[i], 0,
						*static_cast<Uint8 *>(maplpDDSTiles[TRANSPBLOCK]->pixels));
			break;
		case 16:
			for(i=0; i<mapnumblockgfx; i++)
				SDL_SetColorKey(maplpDDSTiles[i], 0,
						*static_cast<Uint16 *>(maplpDDSTiles[TRANSPBLOCK]->pixels));
			break;
		case 24:
		case 32:
			for(i=0; i<mapnumblockgfx; i++)
				SDL_SetColorKey(maplpDDSTiles[i], 0,
						*static_cast<Uint32 *>(maplpDDSTiles[TRANSPBLOCK]->pixels));
			break;
		default:
			break;
	}

	auto mapxo = XPosition/mapblockwidth;
	auto mapyo = YPosition/mapblockheight;
	auto maphclip = XPosition%mapblockwidth;
	auto mapvclip = YPosition%mapblockheight;	
	
	for(i=0; i<lp_dds->h/mapblockheight+2;i++)
		for(auto j = 0; j<lp_dds->w/mapblockwidth+2;j++)
		{	
			tile_dest_rect.y	= i*mapblockheight-mapvclip;
			tile_dest_rect.x	= j*mapblockwidth-maphclip;
			tile_dest_rect.h	= mapblockheight;
			tile_dest_rect.w	= mapblockwidth;
			
			int numtile = mappt[mapxo+j+((mapyo+i)*mapwidth)];
			
			if (((mapxo+j)<mapwidth) && ((mapyo+i)<mapheight))
			    if (numtile>=0)
				SDL_BlitSurface(maplpDDSTiles[reinterpret_cast<BLKSTR *>(mapblockstrpt + numtile)->bgoff],
					nullptr, lp_dds, &tile_dest_rect);
				else
				{
					auto anim = reinterpret_cast<ANISTR *>(mapanimstrendpt + numtile);
					auto numanim = mapanimstrpt[ANDTSize+anim->ancuroff+1]&0XFF;
					numanim <<= 8;
					numanim |= mapanimstrpt[ANDTSize+anim->ancuroff]&0XFF;
//					numanim >>= 5;
					SDL_BlitSurface(maplpDDSTiles[reinterpret_cast<BLKSTR *>(mapblockstrpt + numanim)->bgoff],
						nullptr, lp_dds, &tile_dest_rect);
				}
		}
	
	return 0;
}

// ---------------------------------------------------------------------------------
// draws the foreground layer WITH Transparency (couleur de transparence = celle du
//			1er block qui doit etre uni)
// ---------------------------------------------------------------------------------
int SDLMappy::map_draw_fg (SDL_Surface * lp_dds )
{
	int			i;
	SDL_Rect tile_dest_rect;
	
	switch(mapdepth)
	{
		case 8:
			for(i=0; i<mapnumblockgfx; i++)
				SDL_SetColorKey(maplpDDSTiles[i], 0,
						*static_cast<Uint8 *>(maplpDDSTiles[TRANSPBLOCK]->pixels));
			break;
		case 16:
			for(i=0; i<mapnumblockgfx; i++)
				SDL_SetColorKey(maplpDDSTiles[i], 0,
						*static_cast<Uint16 *>(maplpDDSTiles[TRANSPBLOCK]->pixels));
			break;
		case 24:
		case 32:
			for(i=0; i<mapnumblockgfx; i++)
				SDL_SetColorKey(maplpDDSTiles[i], 0,
						*static_cast<Uint32 *>(maplpDDSTiles[TRANSPBLOCK]->pixels));
			break;
		default:
			break;
	}

	auto mapxo = XPosition/mapblockwidth;
	auto mapyo = YPosition/mapblockheight;
	auto maphclip = XPosition%mapblockwidth;
	auto mapvclip = YPosition%mapblockheight;	
	
	for(i=0; i<lp_dds->h/mapblockheight+2;i++)
		for(auto j = 0; j<lp_dds->w/mapblockwidth+2;j++)
		{	
			tile_dest_rect.y	= i*mapblockheight-mapvclip;
			tile_dest_rect.x	= j*mapblockwidth-maphclip;
			tile_dest_rect.h	= mapblockheight;
			tile_dest_rect.w	= mapblockwidth;
			
			int numtile = mappt[mapxo+j+((mapyo+i)*mapwidth)];
			
			if (((mapxo+j)<mapwidth) && ((mapyo+i)<mapheight))
			    if (numtile>=0)
				SDL_BlitSurface(maplpDDSTiles[reinterpret_cast<BLKSTR *>(mapblockstrpt + numtile)->fgoff],
					nullptr, lp_dds, &tile_dest_rect);
				else
				{
					auto anim = reinterpret_cast<ANISTR *>(mapanimstrendpt + numtile);
					auto numanim = mapanimstrpt[ANDTSize+anim->ancuroff+1]&0XFF;
					numanim <<= 8;
					numanim |= mapanimstrpt[ANDTSize+anim->ancuroff]&0XFF;
//					numanim >>= 5;
					SDL_BlitSurface(maplpDDSTiles[reinterpret_cast<BLKSTR *>(mapblockstrpt + numanim)->fgoff],
						nullptr, lp_dds, &tile_dest_rect);
				}
		}
	
	return 0;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
void SDLMappy::map_init_anims () const
{
	if (mapanimstrpt==nullptr) return;
	auto myanpt = reinterpret_cast<ANISTR *>(mapanimstrendpt); myanpt--;
	while (myanpt->antype!=-1)
	{
		if (myanpt->antype==AN_PPFR) myanpt->antype = AN_PPFF;
		if (myanpt->antype==AN_PPRF) myanpt->antype = AN_PPRR;
		if (myanpt->antype==AN_ONCES) myanpt->antype = AN_ONCE;
		if ((myanpt->antype==AN_LOOPR) || (myanpt->antype==AN_PPRR))
		{
		myanpt->ancuroff = myanpt->anstartoff;
		if ((myanpt->anstartoff)!=(myanpt->anendoff)) myanpt->ancuroff=(myanpt->anendoff)-4;
		} else {
		myanpt->ancuroff = myanpt->anstartoff;
		}
		myanpt->ancount = myanpt->andelay;
		myanpt--;
	}
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
void SDLMappy::map_update_anims () const
{
	if (mapanimstrpt==nullptr) return;
	auto myanpt = reinterpret_cast<ANISTR *>(mapanimstrendpt); myanpt--;
	while (myanpt->antype!=-1)
	{
		if (myanpt->antype!=AN_NONE) { myanpt->ancount--; if (myanpt->ancount<0) {
		myanpt->ancount = myanpt->andelay;
		if (myanpt->antype==AN_LOOPF)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff+=4;
			if (myanpt->ancuroff==myanpt->anendoff) myanpt->ancuroff = myanpt->anstartoff;
		} }
		if (myanpt->antype==AN_LOOPR)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff-=4;
			if (myanpt->ancuroff==((myanpt->anstartoff)-4))
				myanpt->ancuroff = (myanpt->anendoff)-4;
		} }
		if (myanpt->antype==AN_ONCE)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff+=4;
			if (myanpt->ancuroff==myanpt->anendoff) { myanpt->antype = AN_ONCES;
				myanpt->ancuroff = myanpt->anstartoff; }
		} }
		if (myanpt->antype==AN_ONCEH)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) {
			if (myanpt->ancuroff!=((myanpt->anendoff)-4)) myanpt->ancuroff+=4;
		} }
		if (myanpt->antype==AN_PPFF)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff+=4;
			if (myanpt->ancuroff==myanpt->anendoff) { myanpt->ancuroff -= 8;
			myanpt->antype = AN_PPFR;
			if (myanpt->ancuroff<myanpt->anstartoff) myanpt->ancuroff +=4; }
		} } else {
		if (myanpt->antype==AN_PPFR)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff-=4;
			if (myanpt->ancuroff==((myanpt->anstartoff)-4)) { myanpt->ancuroff += 8;
			myanpt->antype = AN_PPFF;
			if (myanpt->ancuroff>myanpt->anendoff) myanpt->ancuroff -=4; }
		} } }
		if (myanpt->antype==AN_PPRR)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff-=4;
			if (myanpt->ancuroff==((myanpt->anstartoff)-4)) { myanpt->ancuroff += 8;
			myanpt->antype = AN_PPRF;
			if (myanpt->ancuroff>myanpt->anendoff) myanpt->ancuroff -=4; }
		} } else {
		if (myanpt->antype==AN_PPRF)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff+=4;
			if (myanpt->ancuroff==myanpt->anendoff) { myanpt->ancuroff -= 8;
			myanpt->antype = AN_PPRR;
			if (myanpt->ancuroff<myanpt->anstartoff) myanpt->ancuroff +=4; }
		} } }
	} } myanpt--; }
}

// ---------------------------------------------------------------------------------
// creates a parallax surface from a bitmap file
// returns true, if an error occurs, returns FALSE
// errors can be: bitmap not found
// caution: the parallax surface size must be a multiple of the map block size
// ---------------------------------------------------------------------------------
SDL_bool SDLMappy::create_parallax( char * filename )
{
    // set the global variable to the filename
    ParallaxFilename = new char[ strlen(filename)+1 ];
    strcpy_s( ParallaxFilename, strlen(filename), filename );
    return restore_parallax( );
}

// ---------------------------------------------------------------------------------
// creates a parallax surface from a bitmap file
// returns true, if an error occurs, returns FALSE
// errors can be: bitmap not found
// caution: the parallax surface size must be a multiple of the map block size
// ---------------------------------------------------------------------------------
SDL_bool SDLMappy::restore_parallax()
{
	// when no map is loaded, return an error
    if( get_map_width() == 0 )
        return SDL_FALSE;

    // load the bitmap into a surface
	if(ParallaxFilename == nullptr)
		return SDL_FALSE;
	
	/* Load a BMP sprite into a surface */
	auto source_surface_tmp = SDL_LoadBMP(ParallaxFilename);
	if ( source_surface_tmp == nullptr )
		return SDL_FALSE;
		
	/* Convert the sprite to the video format (maps colors) */
	//ParallaxSurface = SDL_DisplayFormat(SourceSurfaceTmp);
	SDL_FreeSurface(source_surface_tmp);
	if ( ParallaxSurface == nullptr )
		return SDL_FALSE;

    PARAW = ParallaxSurface->w;
    PARAH = ParallaxSurface->h;


    return SDL_TRUE;
}

int SDLMappy::draw_parallax (SDL_Surface * lp_dds ) const
{
	SDL_Rect tile_dest_rect;

	auto mapxo = XPosition;
	auto mapyo = YPosition;
	auto indentx = ((mapxo/2)%PARAW);
	auto indenty = ((mapyo/2)%PARAH);
	
	for(auto i = 0; i<lp_dds->h/PARAH+2;i++)
		for(auto j = 0; j<lp_dds->w/PARAW+2;j++)
		{	
			tile_dest_rect.y	= i*PARAH-indenty;
			tile_dest_rect.x	= j*PARAW-indentx;
			tile_dest_rect.h	= PARAH;
			tile_dest_rect.w	= PARAW;
			
			SDL_BlitSurface(ParallaxSurface, nullptr, lp_dds, &tile_dest_rect);
		}
	
	return 0;
}
