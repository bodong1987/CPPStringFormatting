1.add CPPFormatLibary to External, add search path in c# files.
2.add UnFormat.h and UnFormatPost.h to Core/Inc
3.add #include "UnFormat.h" to the beginnng of UnString.h
4.add _IMPLEMENT_FORMAT_FOR_FSTRING(); to the public area for FString class content
5.add #include "UnFormatPost.h" after UnName.h
6.enjoy it!!