#ifndef DONSUS_BUILD_CONTEXT
#define DONSUS_BUILD_CONTEXT

#if defined(_WIN32) || defined(_WIN64)
	#ifndef DU_SYSTEMS_WINDOWS	
	#define DU_SYSTEMS_WINDOWS
	#endif

#elif defined(__APPLE__) && defined(__MACH__)
	#ifndef DU_SYSTEMS_OSX
	#define DU_SYSTEMS_OSX 1
	#endif

#elif defined(__unix__)
	#ifndef DU_SYSTEMS_UNIX
	#define DU_SYSTEMS_UNIX 1
	#endif
	
	#if defined(__linux__)

		#ifndef DU_SYSTEMS_LINUX
		#define DU_SYSTEMS_LINUX 1
		#endif

	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
		#ifndef DU_SYSTEM_FREEBSD
		#define DU_SYSTEM_FREEBSD 1
		#endif

	#elif defined(__OpenBSD__)
		#ifndef DU_SYSTEM_OPENBSD
		#define DU_SYSTEM_OPENBSD 1
		#endif

	#elif defined(__NetBSD__)
		#ifndef DU_SYSTEM_NETBSD
		#define DU_SYSTEM_NETBSD 1
		#endif

	#elif defined(__HAIKU__) || defined(__haiku__)
		#ifndef DU_SYSTEM_HAIKU
		#define DU_SYSTEM_HAIKU 1
		#endif
	#else
		#error This Unix operating system is not supported
	#endif
#else
	#error This operating system is not supported
#endif

#if defined(_MSC_VER)
	#define DU_COMPILER_MSVC 1
#elif defined(__GNUC__)
	#define DU_COMPILER_GCC 1
#elif defined(__clang__)
	#define DU_COMPILER_CLANG 1
#else 
	#error Unknown compiler
#endif

namespace DonsusBuildContext {

			
     bool has_ansi_colours() {
		 // https://force-color.org/
		 char const* force_colour = getenv("FORCE_COLOR");
		 if (force_colour != nullptr) {
			 return true;
		 }
		 return false;
#if defined(DU_SYSTEMS_WINDOWS)
#endif
#elif defined(DU_SYSTEMS_OSX) || defined(GDU_SYSTEMS_UNIX)

#endif
	}
}
