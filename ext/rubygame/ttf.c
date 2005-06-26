/*
	Rubygame -- Ruby code and bindings to SDL to facilitate game creation
	Copyright (C) 2004  John 'jacius' Croisant

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "rubygame.h"

#ifdef HAVE_SDL_TTF_H
#include "SDL_ttf.h"

#ifndef SDL_TTF_MAJOR_VERSION
#define SDL_TTF_MAJOR_VERSION 0
#endif

#ifndef SDL_TTF_MINOR_VERSION
#define SDL_TTF_MINOR_VERSION 0
#endif

#ifndef SDL_TTF_PATCHLEVEL
#define SDL_TTF_PATCHLEVEL 0
#endif

/*
 *  call-seq:
 *     Rubygame::TTF.version  =>  [major,minor,patch]
 *
 *  Return the major, minor, and patch numbers for the version of SDL_ttf 
 *  that Rubygame was compiled with. If Rubygame was not compiled with SDL_ttf,
 *  all version numbers will be +0+ (zero), and you should not attempt to use
 *  the TTF class.
 */
VALUE rbgm_ttf_version(VALUE module)
{ 
  return rb_ary_new3(3,
					 INT2NUM(SDL_TTF_MAJOR_VERSION),
					 INT2NUM(SDL_TTF_MINOR_VERSION),
					 INT2NUM(SDL_TTF_PATCHLEVEL));
}

/*
 *  call-seq:
 *     Rubygame::TTF.setup  =>  nil
 *
 *  Attempt to setup the TTF class for use by initializing SDL_ttf.
 *  This _must_ be called before the TTF class can be used.
 *  Raises SDLError if there is a problem initializing SDL_ttf.
 */
VALUE rbgm_ttf_setup(VALUE module)
{
	if(TTF_Init()!=0)
		rb_raise(eSDLError,"could not setup TTF class: %s",TTF_GetError());
	return Qnil;
}

/*
 *  call-seq:
 *     Rubygame::TTF.quit  =>  nil
 *
 *  Clean up and quit SDL_ttf, making the TTF class unusable as a result
 *  (until it is setup again). This does not need to be called before Rubygame
 *  exits, as it will be done automatically.
 */
VALUE rbgm_ttf_quit(VALUE module)
{
	if(TTF_WasInit())
		TTF_Quit();
	return Qnil;
}

/*
 *  call-seq:
 *     Rubygame::TTF.new( file, size )  =>  TTF
 *
 *  Create a new TTF object, which can render text to a Surface with a
 *  particular font style and size.
 *
 *  This function takes these arguments:
 *  - file:: filename of the TrueType font to use. Should be a +.TTF+ or 
 *           +.FON+ file.
 *  - size:: point size (based on 72DPI). Or, the height in pixels from the
 *           bottom of the descent to the top of the ascent.
 */
VALUE rbgm_ttf_new(int argc, VALUE *argv, VALUE class)
{
	VALUE self;
	TTF_Font *font;
	
	if(!TTF_WasInit())
		rb_raise(eSDLError,"Font module must be initialized before making new font.");
	font = TTF_OpenFont(StringValuePtr(argv[0]), NUM2INT(argv[1]));
	if(font == NULL)
		rb_raise(eSDLError,"could not load font: %s",TTF_GetError());

	self = Data_Wrap_Struct(cTTF,0,TTF_CloseFont,font);
	rb_obj_call_init(self,argc,argv);
	return self;
}

VALUE rbgm_ttf_initialize(int argc, VALUE *argv, VALUE self)
{ 
	return self;
}

/*  call-seq:
 *     TTF#bold  =>  value
 *
 *  True if bolding is enabled for the font.
 */
VALUE rbgm_ttf_getbold(VALUE self)
{
	TTF_Font *font;
	int style;

	Data_Get_Struct(self,TTF_Font,font);
	style = TTF_GetFontStyle(font);
	if((style & TTF_STYLE_BOLD) == TTF_STYLE_BOLD)
		return Qtrue;
	else
		return Qfalse;
}

/*  call-seq:
 *     TTF#bold = value  =>  old_value
 *
 *  Set whether bolding is enabled for this font. Returns the old value.
 */
VALUE rbgm_ttf_setbold(VALUE self,VALUE bold)
{
	TTF_Font *font;
	int style;

	Data_Get_Struct(self,TTF_Font,font);
	style = TTF_GetFontStyle(font);

	/* Font is currently bold, and we want it to be not bold. */
	if((style & TTF_STYLE_BOLD) == TTF_STYLE_BOLD && !bold)
	  {
		TTF_SetFontStyle(font,style^TTF_STYLE_BOLD);
		return Qtrue;			/* The old value */
	  }
	/* Font is not currently bold, and we want it to be bold. */
	else if(bold)
	  {
		TTF_SetFontStyle(font,style|TTF_STYLE_BOLD);
		return Qfalse;			/* The old value */
	  }
	/* No changes were necessary. */
	return bold;				/* Same as old value */
}

/*  call-seq:
 *     TTF#italic  =>  value
 *
 *  True if italicizing is enabled for the font.
 */
VALUE rbgm_ttf_getitalic(VALUE self)
{
	TTF_Font *font;
	int style;

	Data_Get_Struct(self,TTF_Font,font);
	style = TTF_GetFontStyle(font);
	if((style & TTF_STYLE_ITALIC) == TTF_STYLE_ITALIC)
		return Qtrue;
	else
		return Qfalse;
}

/*  call-seq:
 *     TTF#italic = value  =>  old_value
 *
 *  Set whether italicizing is enabled for this font. Returns the old value.
 */
VALUE rbgm_ttf_setitalic(VALUE self,VALUE italic)
{
	TTF_Font *font;
	int style;

	Data_Get_Struct(self,TTF_Font,font);
	style = TTF_GetFontStyle(font);

	/* Font is currently italic, and we want it to be not italic. */
	if((style & TTF_STYLE_ITALIC) == TTF_STYLE_ITALIC && !italic)
	  {
		TTF_SetFontStyle(font,style^TTF_STYLE_ITALIC);
		return Qtrue;			/* The old value */
	  }
	/* Font is not currently italic, and we want it to be italic. */
	else if(italic)
	  {
		TTF_SetFontStyle(font,style|TTF_STYLE_ITALIC);
		return Qfalse;			/* The old value */
	  }
	/* No changes were necessary. */
	return italic;				/* Same as old value */
}

/*  call-seq:
 *     TTF#underline  =>  value
 *
 *  True if underlining is enabled for the font.
 */
VALUE rbgm_ttf_getunderline(VALUE self)
{
	TTF_Font *font;
	int style;

	Data_Get_Struct(self,TTF_Font,font);
	style = TTF_GetFontStyle(font);
	if((style & TTF_STYLE_UNDERLINE) == TTF_STYLE_UNDERLINE)
		return Qtrue;
	else
		return Qfalse;
}

/*  call-seq:
 *     TTF#underline = value  =>  old_value
 *
 *  Set whether underlining is enabled for this font. Returns the old value.
 */
VALUE rbgm_ttf_setunderline(VALUE self,VALUE underline)
{
	TTF_Font *font;
	int style;

	Data_Get_Struct(self,TTF_Font,font);
	style = TTF_GetFontStyle(font);

	/* Font is currently underlined, and we want it to be not underlined. */
	if((style & TTF_STYLE_UNDERLINE) == TTF_STYLE_UNDERLINE && !underline)
	  {
		TTF_SetFontStyle(font,style^TTF_STYLE_UNDERLINE);
		return Qtrue;			/* The old value */
	  }
	/* Font is not currently underlined, and we want it to be underlined. */
	else if(underline)
	  {
		TTF_SetFontStyle(font,style|TTF_STYLE_UNDERLINE);
		return Qfalse;			/* The old value */
	  }
	/* No changes were necessary. */
	return underline;			/* Same as old value */
}

/*  call-seq:
 *     TTF#height  =>  value
 *
 *  Return the biggest height (bottom to top; in pixels) of all glyphs in the 
 *  font.
 */
VALUE rbgm_ttf_height(VALUE self)
{
	TTF_Font *font;
	Data_Get_Struct(self,TTF_Font,font);
	return INT2NUM(TTF_FontHeight(font));
}

/*  call-seq:
 *     TTF#ascent  =>  value
 *
 *  Return the biggest ascent (baseline to top; in pixels) of all glyphs in 
 *  the font.
 */
VALUE rbgm_ttf_ascent(VALUE self)
{
	TTF_Font *font;
	Data_Get_Struct(self,TTF_Font,font);
	return INT2NUM(TTF_FontAscent(font));
}

/*  call-seq:
 *     TTF#descent  =>  value
 *
 *  Return the biggest descent (baseline to bottom; in pixels) of all glyphs in
 *  the font.
 */
VALUE rbgm_ttf_descent(VALUE self)
{
	TTF_Font *font;
	Data_Get_Struct(self,TTF_Font,font);
	return INT2NUM(TTF_FontDescent(font));
}

/*  call-seq:
 *     TTF#lineskip  =>  value
 *
 *  Return the recommended distance from a point on a line of text to the
 *  same point on the line of text below it.
 */
VALUE rbgm_ttf_lineskip(VALUE self)
{
	TTF_Font *font;
	Data_Get_Struct(self,TTF_Font,font);
	return INT2NUM(TTF_FontLineSkip(font));
}

/*  call-seq:
 *     TTF#render(string, aa, fg, bg)  =>  Surface
 *
 *  Renders a string to a Surface with the font's style and the given color(s).
 *
 *  This method takes these arguments:
 *  - string:: the text string to render
 *  - aa::     whether or not to anti-aliasing. Enabling this makes the text
 *             look much nicer (smooth curves), but is much slower.
 *  - fg::     the color to render the text as, in the form +[r,g,b]+
 *  - bg::     the color to use as a background for the text. This option can
 *             be omitted to have a transparent background.
 */
VALUE rbgm_ttf_render(int argc, VALUE *argv, VALUE self)
{
	SDL_Surface *surf;
	TTF_Font *font;
	int antialias;
	SDL_Color fore, back; /* foreground and background colors */

	if(argc<3)
		rb_raise(rb_eArgError,"wrong number of arguments (%d for 3)",argc);

	Data_Get_Struct(self,TTF_Font,font);

	antialias = argv[1];
	fore.r = NUM2UINT(rb_ary_entry(argv[2],0));
	fore.g = NUM2UINT(rb_ary_entry(argv[2],1));
	fore.b = NUM2UINT(rb_ary_entry(argv[2],2));

	if(argc>3)
	{
		back.r = NUM2UINT(rb_ary_entry(argv[3],0));
		back.g = NUM2UINT(rb_ary_entry(argv[3],1));
		back.b = NUM2UINT(rb_ary_entry(argv[3],2));
	}

	if(antialias) /* anti-aliasing enabled */
	{
		if(argc>3) /* background color provided */
			surf = TTF_RenderText_Shaded(font,StringValuePtr(argv[0]),fore,back);
		else /* no background color */
			surf = TTF_RenderText_Blended(font,StringValuePtr(argv[0]),fore);
	}
	else /* anti-aliasing not enabled */
	{
		if(argc>3) /* background color provided */	
		{
			/* remove colorkey, set color index 0 to background color */
			SDL_Color colors[] = {back};
			surf = TTF_RenderText_Solid(font,StringValuePtr(argv[0]),fore);
			SDL_SetColors(surf,colors,0,1);
			SDL_SetColorKey(surf,0,0);
		}
		else /* no background color */
		{
			surf = TTF_RenderText_Solid(font,StringValuePtr(argv[0]),fore);
		}
	}

	if(surf==NULL)
		rb_raise(eSDLError,"could not render font object: %s",TTF_GetError());
	return Data_Wrap_Struct(cSurface,0,SDL_FreeSurface,surf);
}

void Rubygame_Init_Font()
{
	mFont = rb_define_module_under(mRubygame,"Font");

	cTTF = rb_define_class_under(mFont,"TTF",rb_cObject);
	rb_define_singleton_method(cTTF,"new",rbgm_ttf_new,-1);
	rb_define_singleton_method(cTTF,"usable?",rbgm_usable,0);
	rb_define_singleton_method(cTTF,"version",rbgm_ttf_version,0);
	rb_define_singleton_method(cTTF,"setup",rbgm_ttf_setup,0);
	rb_define_singleton_method(cTTF,"quit",rbgm_ttf_quit,0);

	rb_define_method(cTTF,"initialize",rbgm_ttf_initialize,-1);
	rb_define_method(cTTF,"bold",rbgm_ttf_getitalic,0);
	rb_define_method(cTTF,"bold=",rbgm_ttf_setitalic,1);
	rb_define_alias( cTTF,"b","bold");
	rb_define_alias( cTTF,"b=","bold=");
	rb_define_method(cTTF,"italic",rbgm_ttf_getitalic,0);
	rb_define_method(cTTF,"italic=",rbgm_ttf_setitalic,1);
	rb_define_alias( cTTF,"i","italic");
	rb_define_alias( cTTF,"i=","italic=");
	rb_define_method(cTTF,"underline",rbgm_ttf_getunderline,0);
	rb_define_method(cTTF,"underline=",rbgm_ttf_setunderline,1);
	rb_define_alias( cTTF,"u","underline");
	rb_define_alias( cTTF,"u=","underline=");
	rb_define_method(cTTF,"height",rbgm_ttf_height,0);
	rb_define_method(cTTF,"ascent",rbgm_ttf_ascent,0);
	rb_define_method(cTTF,"descent",rbgm_ttf_descent,0);
	rb_define_method(cTTF,"line_skip",rbgm_ttf_lineskip,0);
	rb_define_method(cTTF,"render",rbgm_ttf_render,-1);
}

/* -- */

#else /* HAVE_SDL_TTF_H */

/* We don't have SDL_ttf, so the "version" is [0,0,0] */
VALUE rbgm_font_version(VALUE module)
{ 
  return rb_ary_new3(3,
					 INT2NUM(0),
					 INT2NUM(0),
					 INT2NUM(0));
}

void Rubygame_Init_Font()
{
	mFont = rb_define_module_under(mRubygame,"Font");

	cTTF = rb_define_class_under(mFont,"TTF",rb_cObject);
	rb_define_singleton_method(cTTF,"new",rbgm_dummy,-1);
	rb_define_singleton_method(cTTF,"usable?",rbgm_unusable,0);
	rb_define_singleton_method(cTTF,"version",rbgm_font_version,0);
	rb_define_singleton_method(cTTF,"setup",rbgm_dummy,-1);
	rb_define_singleton_method(cTTF,"quit",rbgm_dummy,-1);

	rb_define_method(cTTF,"initialize",rbgm_dummy,-1);
	/* No TTF objects can be made, so the other funcs are unneeded. Maybe. */
}

#endif /* HAVE_SDL_TTF_H */