#if defined(_MSC_VER)
#	pragma warning(disable : 4365)
#	pragma warning(disable : 4820)
#	pragma warning(disable : 5045)
#	pragma warning(disable : 5219)
#	pragma warning(disable : 5262)
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#define STB_TRUETYPE_IMPLEMENTATION
//#include "stb_truetype.h"