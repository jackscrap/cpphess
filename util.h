#include <vector>
#include <sstream>

#include "debugTimer.h"

namespace Util {
	std::vector<std::string> split(
		const std::string& s,
		char delim
	) {
		std::vector<std::string> elem;
		
		unsigned int
			start = 0,
			end = 0;
		
		while (end <= s.length(
			)) {
			while (end <= s.length(
			)) {
				if (s.c_str(
				)[end] == delim) {
					break;
				}

				end++;
			}
			
			elem.push_back(
				s.substr(
					start,
					end - start
				)
			);

			start = end + 1;
			end = start;
		}
		
		return elem;
	}
};
