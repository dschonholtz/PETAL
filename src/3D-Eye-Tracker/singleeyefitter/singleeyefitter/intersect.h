#ifndef __INTERSECT_H__
#define __INTERSECT_H__

#include <iostream>
#include <boost/range.hpp>
#include <Eigen/Core>

namespace singleeyefitter {

// Finds the intersection of 2 lines in 2D
template<typename Scalar>
Eigen::Matrix<Scalar,2,1> intersect(const Eigen::ParametrizedLine<Scalar, 2>& line1, const Eigen::ParametrizedLine<Scalar, 2>& line2) {
    Scalar x1 = line1.origin().x();
    Scalar y1 = line1.origin().y();
    Scalar x2 = (line1.origin()+line1.direction()).x();
    Scalar y2 = (line1.origin()+line1.direction()).y();
    Scalar x3 = line2.origin().x();
    Scalar y3 = line2.origin().y();
    Scalar x4 = (line2.origin()+line2.direction()).x();
    Scalar y4 = (line2.origin()+line2.direction()).y();

    Scalar denom = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);

    Scalar px = ((x1*y2 - y1*x2)*(x3-x4) - (x1-x2)*(x3*x4 - y3*x4))/denom;
    Scalar py = ((x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*x4 - y3*x4))/denom;

    return Eigen::Matrix<Scalar,2,1>(px,py);
}

template<typename Range>
typename boost::range_value<Range>::type::VectorType nearest_intersect(const Range& lines);

namespace detail {
    template<typename Scalar, int Dim>
    struct intersect_helper {
        static Eigen::Matrix<Scalar,Dim,1> nearest(const Eigen::ParametrizedLine<Scalar, Dim>& line1, const Eigen::ParametrizedLine<Scalar, Dim>& line2) {
            std::vector<Eigen::ParametrizedLine<Scalar, Dim>> lines;
            lines.push_back(line1);
            lines.push_back(line2);
            return nearest_intersect(lines);
        }
    };

    // Specialise for 2D
    template<typename Scalar>
    struct intersect_helper<Scalar,2> {
        static Eigen::Matrix<Scalar,2,1> nearest(const Eigen::ParametrizedLine<Scalar, 2>& line1, const Eigen::ParametrizedLine<Scalar, 2>& line2) {
            return intersect<Scalar>(line1, line2);
        }
    };
}

// Finds the intersection (in a least-squares sense, or exact in 2D) of 2 lines
template<typename Scalar, int Dim>
Eigen::Matrix<Scalar,Dim,1> nearest_intersect(const Eigen::ParametrizedLine<Scalar, Dim>& line1, const Eigen::ParametrizedLine<Scalar, Dim>& line2) {
    return detail::intersect_helper<Scalar,Dim>::nearest(line1, line2);
}

// Finds the intersection (in a least-squares sense) of multiple lines
template<typename Range>
typename boost::range_value<Range>::type::VectorType nearest_intersect(const Range& lines) {
    typedef typename boost::range_value<Range>::type::VectorType Vector;
    typedef typename Eigen::Matrix<typename Vector::Scalar, Vector::RowsAtCompileTime, Vector::RowsAtCompileTime> Matrix;

    static_assert(Vector::ColsAtCompileTime == 1, "Requires column vector");

    size_t N = lines.size();

    std::vector<Matrix> Ivv;
    Matrix A = Matrix::Zero();
    Vector b = Vector::Zero();
    size_t i = 0;
    for (auto& line : lines) {
        Vector vi = line.direction();
        Vector pi = line.origin();

        Matrix Ivivi = Matrix::Identity() - vi * vi.transpose();

        Ivv.push_back(Ivivi);

        A += Ivivi;
        b += (Ivivi * pi);

        i++;
    }


    Vector x = A.partialPivLu().solve(b);


    /*if nargout == 2
        % Calculate error term
        for i = 1:n
        pi = p(i,:)';
        e = e + ( (x-pi)' * Ivv(:,:,i) * (x-pi) );
        end
        end*/

    return x;
    }

class no_intersection_exception : public std::runtime_error {
public:
    template<typename Scalar>
    no_intersection_exception(const Eigen::ParametrizedLine<Scalar, 3>& line, const Sphere<Scalar>& sphere)
        : std::runtime_error("Line and sphere do not intersect")
    {
    }
};

// Finds the intersection of a line and a sphere
template<typename Scalar>
std::pair<Eigen::Matrix<Scalar, 3, 1>, Eigen::Matrix<Scalar, 3, 1>> intersect(const Eigen::ParametrizedLine<Scalar, 3>& line, const Sphere<Scalar>& sphere) {
    using std::sqrt;
    using math::sq;

    typedef typename Eigen::ParametrizedLine<Scalar, 3>::VectorType Vector;
	
	// Yuta
	if (!(std::abs(line.direction().norm() - 1) < 0.0001)){
        throw no_intersection_exception(line, sphere);
	}

    Vector v = line.direction();
    // Put p at origin
    Vector p = line.origin();
    Vector c = sphere.centre - p;
    Scalar r = sphere.radius;

    // From Wikipedia :)
    Scalar vcvc_cc_rr = sq(v.dot(c)) - c.dot(c) + sq(r);
    if (vcvc_cc_rr < 0) {
        throw no_intersection_exception(line, sphere);
    }

    Scalar s1 = v.dot(c) - sqrt(vcvc_cc_rr);
    Scalar s2 = v.dot(c) + sqrt(vcvc_cc_rr);

    Vector p1 = p + s1*v;
    Vector p2 = p + s2*v;

    return std::make_pair(p1, p2);
}

}

#endif//__INTERSECT_H__
