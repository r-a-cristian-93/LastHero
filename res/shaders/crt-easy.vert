/*
   CRT - Guest
   edited by metallic77

   Copyright (C) 2017-2018 guest(r) - guest.r@gmail.com

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

// Parameter lines go here:
#version 130

#pragma parameter brightboost "Bright boost" 1.0 0.5 2.0 0.05
#pragma parameter sat "Saturation adjustment" 1.0 0.0 2.0 0.05
#pragma parameter scanline "Scanline adjust" 8.0 1.0 12.0 1.0
#pragma parameter beam_min "Scanline dark" 1.35 0.5 3.0 0.05
#pragma parameter beam_max "Scanline bright" 1.05 0.5 3.0 0.05
#pragma parameter h_sharp "Horizontal sharpness" 2.00 1.0 5.0 0.05
#pragma parameter gamma_out "Gamma out" 0.5 0.2 0.6 0.01
#pragma parameter shadowMask "CRT Mask: 0:CGWG, 1-4:Lottes, 5-6:'Trinitron'" 0.0 -1.0 10.0 1.0
#pragma parameter masksize "CRT Mask Size (2.0 is nice in 4k)" 1.0 1.0 2.0 1.0
#pragma parameter mcut "Mask 5-7-10 cutoff" 0.2 0.0 0.5 0.05
#pragma parameter maskDark "Lottes maskDark" 0.5 0.0 2.0 0.1
#pragma parameter maskLight "Lottes maskLight" 1.5 0.0 2.0 0.1
#pragma parameter CGWG "CGWG Mask Str." 0.4 0.0 1.0 0.1
#pragma parameter warpX "warpX" 0.0 0.0 0.125 0.01
#pragma parameter warpY "warpY" 0.0 0.0 0.125 0.01
#pragma parameter vignette "Vignette On/Off" 0.0 0.0 1.0 1.0
#pragma parameter vpower "Vignette Power" 0.2 0.0 1.0 0.01
#pragma parameter vstr "Vignette strength" 40.0 0.0 50.0 1.0

#if __VERSION__ >= 130
#define COMPAT_VARYING out
#define COMPAT_ATTRIBUTE in
#define COMPAT_TEXTURE texture
#else
#define COMPAT_VARYING varying
#define COMPAT_ATTRIBUTE attribute
#define COMPAT_TEXTURE texture2D
#endif

#ifdef GL_ES
#define COMPAT_PRECISION mediump
#else
#define COMPAT_PRECISION
#endif

//COMPAT_ATTRIBUTE vec4 VertexCoord;
COMPAT_ATTRIBUTE vec4 COLOR;
//COMPAT_ATTRIBUTE vec4 TexCoord;
COMPAT_VARYING vec4 COL0;
COMPAT_VARYING vec4 TEX0;

vec4 _oPosition1;
//uniform mat4 MVPMatrix;
uniform COMPAT_PRECISION int FrameDirection;
uniform COMPAT_PRECISION int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize = vec2(1024, 768);
uniform COMPAT_PRECISION vec2 TextureSize = vec2(1024, 768);
uniform COMPAT_PRECISION vec2 InputSize = vec2(1024, 768);

void main()
{
	mat4 MVPMatrix = gl_ModelViewProjectionMatrix;
	vec4 VertexCoord = gl_Vertex;
	vec4 TexCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;


    gl_Position = MVPMatrix * VertexCoord;
    COL0 = COLOR;
    TEX0.xy = TexCoord.xy * 1.00001;
}
