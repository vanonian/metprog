#include <fstream>
#include <iostream>
#include <sstream>

#include <string>
#include <tuple>
#include <vector>


template <size_t Pos, typename ...Args>
class CSVParser {
  public:
    void parse(std::tuple<Args...>& CSVrow, std::istringstream& iss) {
      CSVParser<Pos - 1, Args...> parser;
      parser.parse(CSVrow, iss);

      std::string s;
      std::getline(iss, s, ',');
      read(std::get<Pos>(CSVrow), std::istringstream(s));
    }

  private:
	   template<typename T>
	   void read(T& t, std::istringstream& iss) { iss >> t; }

     //String with whitespaces
     void read(std::string& t, std::istringstream& iss) {
		     std::getline(iss >> std::ws, t);
	   }
};

template <typename ...Args>
class CSVParser<0, Args...> {
  public:
    void parse(std::tuple<Args...>& CSVrow, std::istringstream& iss) {
      std::string s;
      std::getline(iss, s, ',');
      std::istringstream(s) >> std::get<0>(CSVrow);
    }
  private:
    template<typename T>
  	void read(T& t, std::istringstream& iss) { iss >> t; }

    //String with whitespaces
    void read(std::string& t, std::istringstream& iss) {
      std::getline(iss >> std::ws, t);
  	}
};


template <typename ...Args>
std::vector<std::tuple<Args...>> ReadCSV(const std::string& filename) {
  std::ifstream ifs;
  ifs.open(filename, std::ifstream::in);

  CSVParser<sizeof ...(Args) - 1, Args...> parser;

  std::string row;
  std::tuple<Args...> CSVrow;
  std::vector<std::tuple<Args...>> CSVdata;

  while (std::getline(ifs, row)) {
    std::istringstream iss(row);
    parser.parse(CSVrow, iss);
    CSVdata.push_back(CSVrow);
  }

  ifs.close();
  return CSVdata;
}

template <typename ...Args>
std::vector<std::tuple<Args...>> ReadCSV(const char* filename) {
  return ReadCSV<Args...>(std::string(filename));
}
