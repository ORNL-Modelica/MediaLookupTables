#include <noname.h>

#include <fstream>
#include <iostream>
#include <string>

int main() {
  std::string const prefix = "/scratch/"; // edit this line
  std::string const fx = prefix + "p.csv";
  std::string const fy = prefix + "T.csv";
  std::string const fz = prefix + "h.csv";
  char const sep = '?';
  std::string const k = sep + fx + sep + fy + sep + fz;
  std::fstream sx(fx);
  std::fstream sy(fy);
  std::fstream sz(fz);
  std::string e;
  for (int i = 0; i < 100; ++i) {
    std::getline(sx, e, ',');
    double const x = std::stod(e);
    std::getline(sy, e, ',');
    double const y = std::stod(e);
    std::getline(sz, e, ',');
    double const z_ref = std::stod(e);
    double const z_eval1 = bilinear_eval(k.c_str(), x, y);
    double const z_eval3 = bicubic_eval(k.c_str(), x, y);
    std::cout << i << "  " << x << "  " << y << "  " << z_ref << "  " << z_eval1
              << "  " << z_eval3 << "\n";
  }
  return 0;
}
