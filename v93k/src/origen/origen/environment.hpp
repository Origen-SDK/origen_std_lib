#ifndef ORIGEN_ENVIRONMENT_INCLUDED
#define ORIGEN_ENVIRONMENT_INCLUDED

#include <iostream>

using namespace std;

namespace Origen {
	// This is a singleton (available via Origen.env()) which is used as a global
	// storage area by the Origen libs
	class Environment
	{
		public:
			// http://stackoverflow.com/questions/270947/can-any-one-provide-me-a-sample-of-singleton-in-c/271104#271104
			static Environment& getInstance()
			{
				static Environment instance;
				return instance;
			}

			template < class T >
			void _registerTestMethod(T* ptr) 
			{

				return; 
			}

		private:
			Environment() {
				initialize();
			};
			Environment(Environment const&);    // Stop other instances being created
			void operator=(Environment const&); // Stop other instances being created
			void initialize();
			
	};
}

#endif
