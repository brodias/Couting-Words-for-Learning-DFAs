#include <iostream>
#include <Eigen/Dense>

int main() {
  // Define two matrices, both 3x3
  Eigen::Matrix3d p;
  Eigen::Matrix3d q;

  // Define two three-dimensional vectors
  // The constructor provides initialisation
  Eigen::Vector3d r(1,2,3);
  Eigen::Vector3d s(4,5,6);

  // Use the << operator to fill the matrices
  p << 1, 2, 3,
       4, 5, 6,
       7, 8, 9;
  q << 10, 11, 12,
       13, 14, 15,
       16, 17, 18;
       
  std::cout << "r=\n" << r << std::endl;
  std::cout << "s=\n" << s << std::endl;

  std::cout << "p=\n" << p << std::endl;
  std::cout << "q=\n" << q << std::endl;

  std::cout << "r[1]=\n" << r[1] << std::endl;
  
  Eigen::Matrix3d m = Eigen::Matrix3d::Random(3,3);
  std::cout << "m=\n" << m << std::endl;
  std::cout << "m[0]=\n" << m(4) << std::endl;

  // Output arithmetic operations for matrices
  // std::cout << "p+q=\n" << p + q << std::endl;
  // std::cout << "p-q=\n" << p - q << std::endl;

  // Output arithmetic operations for vectors
  // std::cout << "r+s=\n" << r + s << std::endl;
  // std::cout << "r-s=\n" << r - s << std::endl;
  
  // Multiply and divide by a scalar
  // std::cout << "p * 3.14159 =\n" << p * 3.14159 << std::endl;
  // std::cout << "p / 2.71828 =\n" << p / 2.71828 << std::endl; 
  
  
  // Declare a 3x3 matrix with random entries
  // Eigen::Matrix3d m = Eigen::Matrix3d::Random(3,3);

  // std::cout << "m =\n" << m << std::endl;

  // Output the transpose of p
  // std::cout << "m^T =\n" << m.transpose() << std::endl;

  // In-place transposition
  // m.transposeInPlace();

  // Output the in-place transpose of p
  // std::cout << "m^T =\n" << m << std::endl;
  
  // Matrix/matrix multiplication
  // std::cout << "p*p:\n" << p*p << std::endl;

  // Matrix/vector multiplication
  // std::cout << "p*r:\n" << p*r << std::endl;
  // std::cout << "r^T*p:\n" << r.transpose()*p << std::endl;
  
  // Vector/vector multiplication (inner product)
  // std::cout << "r^T*s:\n" << r.transpose()*s << std::endl;
}

