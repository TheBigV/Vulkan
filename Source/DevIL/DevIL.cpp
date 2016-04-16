#include "DevIL.hpp"


KID::DevIL::Initer::Initer()
{
	if(ilGetInteger(IL_VERSION_NUM) < IL_VERSION) throw "IL_VERSION_NUM < IL_VERSION";
	if(iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION) throw "ILU_VERSION_NUM < ILU_VERSION";
	if(ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION) throw "ILUT_VERSION_NUM < ILUT_VERSION";

	ilInit();
	iluInit();
	ilutInit();

	ilEnable(IL_FILE_OVERWRITE);
	ilSetInteger(IL_KEEP_DXTC_DATA,IL_TRUE);
	ilutRenderer(ILUT_OPENGL);
	ilutEnable(ILUT_GL_AUTODETECT_TEXTURE_TARGET);
	ilutEnable(ILUT_OPENGL_CONV);
	ilutEnable(ILUT_GL_USE_S3TC);

	//ilEnable(IL_ORIGIN_SET); ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	if(DevIL::ErrorTest()) throw "[DevIL] errors";

	isInit = true;
}
bool					KID::DevIL::ErrorTest()
{
	ILenum error;
	bool result = false;

	while((error = ilGetError()) != IL_NO_ERROR)
	{
		string code;

		switch(error)
		{
		case IL_INVALID_ENUM:// || ILU_INVALID_ENUM || ILUT_INVALID_ENUM:
			code = "IL_INVALID_ENUM";
			break;
		case IL_OUT_OF_MEMORY://, ILU_OUT_OF_MEMORY, ILUT_OUT_OF_MEMORY:
			code = "IL_OUT_OF_MEMORY";
			break;
		case IL_FORMAT_NOT_SUPPORTED:
			code = "IL_FORMAT_NOT_SUPPORTED";
			break;
		case IL_INVALID_VALUE://, ILU_INVALID_VALUE, ILUT_INVALID_VALUE:
			code = "IL_INVALID_VALUE";
			break;
		case IL_ILLEGAL_OPERATION://, ILU_ILLEGAL_OPERATION, ILUT_ILLEGAL_OPERATION:
			code = "IL_ILLEGAL_OPERATION";
			break;
		case IL_ILLEGAL_FILE_VALUE:
			code = "IL_ILLEGAL_FILE_VALUE";
			break;
		case IL_INVALID_FILE_HEADER:
			code = "IL_INVALID_FILE_HEADER";
			break;
		case IL_INVALID_PARAM://, ILU_INVALID_PARAM, ILUT_INVALID_PARAM:
			code = "IL_INVALID_PARAM";
			break;
		case IL_COULD_NOT_OPEN_FILE: //, ILUT_COULD_NOT_OPEN_FILE:
			code = "IL_COULD_NOT_OPEN_FILE";
			break;
		case IL_INVALID_EXTENSION:
			code = "IL_INVALID_EXTENSION";
			break;
		case IL_FILE_ALREADY_EXISTS:
			code = "IL_FILE_ALREADY_EXISTS";
			break;
		case IL_OUT_FORMAT_SAME:
			code = "IL_OUT_FORMAT_SAME";
			break;
		case IL_STACK_OVERFLOW://, ILUT_STACK_OVERFLOW:
			code = "IL_STACK_OVERFLOW";
			break;
		case IL_STACK_UNDERFLOW://, ILUT_STACK_UNDERFLOW:
			code = "IL_STACK_UNDERFLOW";
			break;
		case IL_INVALID_CONVERSION:
			code = "IL_INVALID_CONVERSION";
			break;
		case IL_LIB_JPEG_ERROR:
			code = "IL_LIB_JPEG_ERROR";
			break;
		case IL_LIB_PNG_ERROR:
			code = "IL_LIB_PNG_ERROR";
			break;
		case IL_UNKNOWN_ERROR:
			code = "IL_UNKNOWN_ERROR";
			break;
		case ILUT_NOT_SUPPORTED:
			code = "ILUT_NOT_SUPPORTED";
			break;
		}

		result = true;
	}
	return result;
}
ILinfo					KID::DevIL::Load(const string& filename)
{
	ILuint	id;

	ilGenImages(1,&id);
	ilBindImage(id);

	if(ilLoadImage(filename.c_str()) != IL_TRUE)
	{
		ilDeleteImages(1, &id);
		throw ("Can't Load Image '" + filename + "'");
	}
	DevIL::ErrorTest();

	ILinfo info;
	{
		info.Id				= id;									// the image's id
		info.Data			= ilGetData();							// the image's data
		info.Width			= ilGetInteger(IL_IMAGE_WIDTH);			// the image's width
		info.Height			= ilGetInteger(IL_IMAGE_HEIGHT);		// the image's height
		info.Depth			= ilGetInteger(IL_IMAGE_DEPTH);			// the image's depth
		info.Bpp			= ilGetInteger(IL_IMAGE_BPP);			// bytes per pixel (not bits) of the image
		info.SizeOfData		= ilGetInteger(IL_IMAGE_SIZE_OF_DATA);	// the total size of the data (in bytes)
		info.Format			= ilGetInteger(IL_IMAGE_FORMAT);		// image format (in IL enum style)
		info.Type			= ilGetInteger(IL_IMAGE_TYPE);			// image type (in IL enum style)
		info.Origin			= ilGetInteger(IL_IMAGE_ORIGIN);		// origin of the image
		info.Palette		= ilGetPalette();						// the image's palette
		info.PalType		= ilGetInteger(IL_PALETTE_BASE_TYPE);	// palette type
		info.PalSize		= ilGetInteger(IL_PALETTE_NUM_COLS);	// palette size
		info.CubeFlags		= ilGetInteger(IL_IMAGE_CUBEFLAGS);		// flags for what cube map sides are present
		info.NumNext		= ilGetInteger(IL_NUM_IMAGES);			// number of images following
		info.NumMips		= ilGetInteger(IL_NUM_MIPMAPS);			// number of mipmaps
		info.NumLayers		= ilGetInteger(IL_NUM_LAYERS);			// number of layers
	}

	DevIL::ErrorTest();

	//ilDeleteImages(1,&id);

	return info;
}































