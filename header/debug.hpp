#ifndef DEBUG_HPP
#define DEBUG_HPP

namespace Debug {
//#define Debug(var)  do{std::cout << #var << " : "; view(var);}while(0)

	template<typename T> void view(std::string msg, const T e) {
		std::cout << msg << e << std::endl;
	}

	template<typename T> void view(const T e) {
		std::cout << e << std::endl;
	}

	template<typename T> void view(const std::vector<T>& v) {
		for(const auto& e : v) {
			std::cout << e << " ";
		}
		std::cout << std::endl;
	}

	template<typename T> void view(std::string msg, const std::vector<T>& v) {
		cout << msg;
		for(const auto& e : v) {
			std::cout << e << " ";
		}
		std::cout << std::endl;
	}

	template<typename T> void view(const std::vector<std::vector<T>>& vv) {
		for(const auto& v : vv) {
			view(v);
		}
	}

	template<typename T> void view(std::string msg, const std::vector<std::vector<T>>& vv) {
		cout << msg << endl;
		for(const auto& v : vv) {
			view(v);
		}
	}

	template<typename T> void view(const std::vector<std::vector<std::vector<T>>>& vvv) {
		for(const auto& vv : vvv) {
			view(vv);
			std::cout << std::endl;
		}
	}
}

#endif
