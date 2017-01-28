#ifndef LIBMACROS_H_
#define LIBMACROS_H_

	#ifdef INTERNAL
		#define EXTERNAL_CLASS
		#define EXTERNAL_FUNC
    #elif LIB_BUILD
		#ifdef WIN32
			#define EXTERNAL_CLASS __declspec(dllexport)
			#define EXTERNAL_FUNC __declspec(dllexport)
		#else
			// Symbolic name for visibility("default") attribute.
			#define EXTERNAL_CLASS
			#define EXTERNAL_FUNC __attribute__((visibility("default")))
		#endif
	#else
		#ifdef WIN32
			#define EXTERNAL_CLASS __declspec(dllimport)
			#define EXTERNAL_FUNC __declspec(dllimport)
		#else
			#define EXTERNAL_CLASS
			#define EXTERNAL_FUNC
		#endif
	#endif

#endif
