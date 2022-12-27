#include<iostream>
#include<fstream>
namespace Logger {
	class BasicLogger {
	public:
		std::ostream logger;
		
		std::ostream& Start(const char* logtype) {
			std::cout << "[" << logtype << "]: ";
			return this->logger;
		}
		std::ostream& Start() {
			this->logger << "[" << "Info" << "]: ";
			return this->logger;
		}

	};
}