// Avisynth v2.5.  Copyright 2002 Ben Rudiak-Gould et al.
// http://www.avisynth.org

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA, or visit
// http://www.gnu.org/copyleft/gpl.html .
//
// Linking Avisynth statically or dynamically with other modules is making a
// combined work based on Avisynth.  Thus, the terms and conditions of the GNU
// General Public License cover the whole combination.
//
// As a special exception, the copyright holders of Avisynth give you
// permission to link Avisynth with independent modules that communicate with
// Avisynth solely through the interfaces defined in avisynth.h, regardless of the license
// terms of these independent modules, and to copy and distribute the
// resulting combined work under terms of your choice, provided that
// every copy of the combined work is accompanied by a complete copy of
// the source code of Avisynth (the version of Avisynth used to produce the
// combined work), being distributed under the terms of the GNU General
// Public License plus this exception.  An independent module is a module
// which is not derived from or based on Avisynth, such as 3rd-party filters,
// import and export plugins, or graphical user interfaces.

#ifndef __Image_Sequence_H__
#define __Image_Sequence_H__

#include "il.h"
#include <avs/win.h>
#include <cassert>
#include <fstream>


class ImageWriter : public GenericVideoFilter
/**
  * Class to write video as a sequence of images
 **/
{
public:
  ImageWriter(PClip _child, const char * _base_name, const int _start, const int _end, const char * _ext, bool _info, IScriptEnvironment* env);
  ~ImageWriter();
  PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);

private:
  void fileWrite(std::ostream & file, const BYTE * srcPtr, const int pitch, const int row_size, const int height);

  bool info;

  char base_name[MAX_PATH + 1];
  const char * ext;
  int start;
  int end;

  bool should_flip;

  BITMAPFILEHEADER fileHeader;
  BITMAPINFOHEADER infoHeader;
};


class ImageReader : public IClip
/**
  * Class to read image sequences into video buffers
 **/
{
public:
  ImageReader(const char * _base_name, const int _start, const int _end,
              const double _fps, bool _use_DevIL, bool _info, const char * _pixel,
              bool _animation, IScriptEnvironment* env);
  ~ImageReader();

  PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);

  void __stdcall GetAudio(void* buf, __int64 start, __int64 count, IScriptEnvironment* env) {}
  const VideoInfo& __stdcall GetVideoInfo() { return vi; }
  bool __stdcall GetParity(int n) { return false; }
  int __stdcall SetCacheHints(int cachehints,int frame_range) { return 0; };

  int  framecopies;

private:
  void fileRead(std::istream & file, BYTE * dstPtr, const int pitch, const int row_size, const int height);
  void BlankFrame(PVideoFrame & frame);
  void BlankApplyMessage(PVideoFrame & frame, const char * text, IScriptEnvironment * env);
  bool checkProperties(std::ifstream & file, PVideoFrame & frame, IScriptEnvironment * env);

  char base_name[MAX_PATH + 1];
  const int start;
  bool use_DevIL;
  bool info;
  bool animation;

  VideoInfo vi;

  char filename[MAX_PATH + 1];
  bool should_flip;

  BITMAPFILEHEADER fileHeader;
  BITMAPINFOHEADER infoHeader;
};


const char *const getErrStr(ILenum err);

// Since devIL isn't threadsafe, we need to ensure that only one thread at the time requests frames
extern CRITICAL_SECTION FramesCriticalSection;
extern volatile long refcount;
extern volatile ILint DevIL_Version;

#endif // __Image_Sequence_H__