#ifndef LIBMACROS_H_
#define LIBMACROS_H_

	#ifdef LIB_BUILD
		#ifdef WIN32
		#else
			// Symbolic name for visibility("default") attribute.
			#define EXTERNAL_CLASS
			#define EXTERNAL_FUNC __attribute__((visibility("default")))
		#endif
	#else
		#ifdef WIN32
			//TODO
		#else
			#define EXTERNAL_CLASS
			#define EXTERNAL_FUNC
		#endif
	#endif

#endif
