#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <vector>

void main_loop(std::istream &is, std::ostream &os, std::mutex &m) {
  while (true) {
    std::string line;
    std::lock_guard<std::mutex> guard(m);
    if (!std::getline(is, line)) return;
    std::reverse(line.begin(), line.end());
    os << line << '\n';
  }
}

int main(int argc, char *argv[]) {
  std::ifstream is("input.txt");
  std::ofstream os("output.txt");
  std::mutex m;
  std::vector<std::thread> threads;
  for (int i=0; i!=16; ++i) {
    std::thread t(main_loop, std::ref(is), std::ref(os), std::ref(m));
    threads.push_back(std::move(t));
  }
  for (std::thread &t : threads) {
    t.join();
  }
}
