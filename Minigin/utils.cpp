#include "MiniginPCH.h"
#include "utils.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>



//-----------------------------------------------------------------
// Rectf Constructors
//-----------------------------------------------------------------
utils::Rectf::Rectf()
	:Rectf{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}

utils::Rectf::Rectf(float left, float bottom, float width, float height)
	: left{ left }
	, bottom{ bottom }
	, width{ width }
	, height{ height }
{
}


#pragma region CollisionFunctionality
bool utils::IsPointInRect( const glm::vec2& p, const Rectf& r )
{
	return ( p.x >= r.left && 
		p.x <= r.left + r.width &&
		p.y >= r.bottom &&
		p.y <= r.bottom + r.height );
}

bool utils::IsOverlapping( const glm::vec2& a, const glm::vec2& b, const Rectf& r )
{
	// if one of the line segment end points is in the rect
	if ( utils::IsPointInRect( a, r ) || utils::IsPointInRect( b, r ) )
	{
		return true;
	}

	HitInfo hitInfo{};
	glm::vec2 vertices[]{ glm::vec2 {r.left, r.bottom},
		glm::vec2{ r.left + r.width, r.bottom },
		glm::vec2{ r.left + r.width, r.bottom + r.height },
		glm::vec2{ r.left, r.bottom + r.height } };

	return Raycast( vertices, 4, a, b, hitInfo );
}

bool utils::IsOverlapping( const Rectf& r1, const Rectf& r2 )
{
	// If one rectangle is on left side of the other
	if ( ( r1.left + r1.width ) < r2.left || ( r2.left + r2.width ) < r1.left )
	{
		return false;
	}

	// If one rectangle is under the other
	if ( r1.bottom > ( r2.bottom + r2.height ) || r2.bottom > ( r1.bottom + r1.height ) )
	{
		return false;
	}

	return true;
}

bool utils::IsPointInPolygon( const glm::vec2& p, const std::vector<glm::vec2>& vertices )
{
	return IsPointInPolygon( p, vertices.data( ), vertices.size( ) );
}

bool utils::IsPointInPolygon( const glm::vec2& p, const glm::vec2* vertices, size_t nrVertices )
{
	if ( nrVertices < 2 )
	{
		return false;
	}
	// 1. First do a simple test with axis aligned bounding box around the polygon
	float xMin{ vertices[0].x };
	float xMax{ vertices[0].x };
	float yMin{ vertices[0].y };
	float yMax{ vertices[0].y };
	for ( size_t idx{ 1 }; idx < nrVertices; ++idx )
	{
		if (xMin > vertices[idx].x)
		{
			xMin = vertices[idx].x;
		}
		if (xMax < vertices[idx].x)
		{
			xMax = vertices[idx].x;
		}
		if (yMin > vertices[idx].y)
		{
			yMin = vertices[idx].y;
		}
		if (yMax < vertices[idx].y)
		{
			yMax = vertices[idx].y;
		}
	}
	if (p.x < xMin || p.x > xMax || p.y < yMin || p.y > yMax)
	{
		return false;
	}

	// 2. Draw a virtual ray from anywhere outside the polygon to the point 
	//    and count how often it hits any side of the polygon. 
	//    If the number of hits is even, it's outside of the polygon, if it's odd, it's inside.
	int numberOfIntersectionPoints{0};
	glm::vec2 p2{ xMax + 10.0f, p.y }; // Horizontal line from point to point outside polygon (p2)

	// Count the number of intersection points
	float lambda1{}, lambda2{};
	for ( size_t i{ 0 }; i < nrVertices; ++i )
	{
		if ( IntersectLineSegments( vertices[i], vertices[( i + 1 ) % nrVertices], p, p2, lambda1, lambda2 ) )
		{
			if ( lambda1 > 0 && lambda1 <= 1 && lambda2 > 0 && lambda2 <= 1 )
			{
				++numberOfIntersectionPoints;
			}
		}
	}
	if (numberOfIntersectionPoints % 2 == 0)
	{
		return false;
	}

	return true;

}

bool utils::IntersectLineSegments( const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2, float& outLambda1, float& outLambda2, float epsilon )
{
	bool intersecting{ false };

	const glm::vec2 p1p2{ p2 - p1 };
	const glm::vec2 q1q2{ q2 - q1 };

	// Cross product to determine if parallel
	const float denom = (p1p2.x * q1q2.y - p1p2.y * q1q2.x);
	
	// Don't divide by zero
	if ( std::abs( denom ) > epsilon )
	{
		intersecting = true;

		const glm::vec2 p1q1{ q1 - p1 };

		const float num1 = p1q1.x * q1q2.y - p1q1.y * q1q2.x;
		const float num2 = p1q1.x * p1p2.y - p1q1.y * p1p2.x;
		outLambda1 = num1 / denom;
		outLambda2 = num2 / denom;
	}
	else // are parallel
	{
		// Connect start points
		const glm::vec2 p1q1{ q1 - p1 };

		// Cross product to determine if segments and the line connecting their start points are parallel, 
		// if so, than they are on a line
		// if not, then there is no intersection
		if (std::abs(p1q1.x * q1q2.y - p1q1.y * q1q2.x ) > epsilon)
		{
			return false;
		}

		// Check the 4 conditions
		outLambda1 = 0;
		outLambda2 = 0;
		if (utils::IsPointOnLineSegment(p1, q1, q2) ||
			utils::IsPointOnLineSegment(p2, q1, q2) ||
			utils::IsPointOnLineSegment(q1, p1, p2) ||
			utils::IsPointOnLineSegment(q2, p1, p2))
		{
			intersecting = true;
		}
	}
	return intersecting;
}

bool utils::Raycast( const std::vector<glm::vec2>& vertices, const glm::vec2& rayP1, const glm::vec2& rayP2, HitInfo& hitInfo )
{
	return Raycast( vertices.data( ), vertices.size( ), rayP1, rayP2, hitInfo );
}

bool utils::Raycast( const glm::vec2* vertices, const size_t nrVertices, const glm::vec2& rayP1, const glm::vec2& rayP2, HitInfo& hitInfo )
{
	if ( nrVertices == 0 )
	{
		return false;
	}

	std::vector<HitInfo> hits;

	Rectf r1, r2;
	// r1: minimal AABB rect enclosing the ray
	r1.left = std::min( rayP1.x, rayP2.x );
	r1.bottom = std::min( rayP1.y, rayP2.y );
	r1.width = std::max( rayP1.x, rayP2.x ) - r1.left;
	r1.height = std::max( rayP1.y, rayP2.y ) - r1.bottom;

	// Line-line intersections.
	for ( size_t idx{ 0 }; idx <= nrVertices; ++idx )
	{
		// Consider line segment between 2 consecutive vertices
		// (modulo to allow closed polygon, last - first vertice)
		glm::vec2 q1 = vertices[( idx + 0 ) % nrVertices];
		glm::vec2 q2 = vertices[( idx + 1 ) % nrVertices];

		// r2: minimal AABB rect enclosing the 2 vertices
		r2.left = std::min( q1.x, q2.x );
		r2.bottom = std::min( q1.y, q2.y );
		r2.width = std::max( q1.x, q2.x ) - r2.left;
		r2.height = std::max( q1.y, q2.y ) - r2.bottom;

		if ( IsOverlapping( r1, r2 ) )
		{
			float lambda1{};
			float lambda2{};
			if ( IntersectLineSegments( rayP1, rayP2, q1, q2, lambda1, lambda2 ) )
			{
				if ( lambda1 > 0 && lambda1 <= 1 && lambda2 > 0 && lambda2 <= 1 )
				{
					HitInfo linesHitInfo{};
					linesHitInfo.lambda = lambda1;
					linesHitInfo.intersectPoint = glm::vec2{ rayP1.x + ((rayP2.x - rayP1.x) * lambda1), rayP1.y + ((rayP2.y - rayP1.y) * lambda1) };
					glm::vec2 temp = q2 - q1;
					const float tempX = temp.x;
					temp.x = -temp.y;
					temp.y = tempX;
					const float length = sqrt(temp.x * temp.x + temp.y * temp.y);
					temp /= length;
					linesHitInfo.normal = temp;
					hits.push_back(linesHitInfo);
				}
			}
		}
	}

	if ( hits.size( ) == 0 )
	{
		return false;
	}

	// Get closest intersection point and copy it into the hitInfo parameter
	hitInfo = *std::min_element
	( 
		hits.begin( ), hits.end( ),
		[]( const HitInfo& first, const HitInfo& last ) 
		{
			return first.lambda < last.lambda; 
		} 
	);
	return true;
}

bool  utils::IsPointOnLineSegment( const glm::vec2& p, const glm::vec2& a, const glm::vec2& b )
{
	const glm::vec2 ap{ p - a }, bp{ p - b };
	// If not on same line, return false
	if ( abs( ap.x * bp.y - ap.y * bp.x ) > 0.001f )
	{
		return false;
	}

	// Both vectors must point in opposite directions if p is between a and b
	if ((ap.x * bp.y + ap.y * bp.x) > 0 )
	{
		return false;
	}

	return true;
}

float  utils::DistPointLineSegment( const glm::vec2& p, const glm::vec2& a, const glm::vec2& b )
{
	const glm::vec2 ab{ b - a };
	const float length = sqrt(ab.x * ab.x + ab.y * ab.y);
	glm::vec2 temp = ab;
	temp.x /= length;
	temp.y /= length;
	const glm::vec2 ap{ p - a };
	const glm::vec2 abNorm{ temp };
	const float distToA{ abNorm.x * ap.y + abNorm.y * ap.x };

	// If distToA is negative, then the closest point is A
	// return the distance a, p
	if ( distToA < 0 )
	{
		return sqrt(ap.x * ap.x + ap.y * ap.y);
	}
	// If distToA is > than dist(a,b) then the closest point is B
	// return the distance b, p
	const float distAB{ sqrt(ab.x * ab.x + ab.y * ab.y) };
	if ( distToA > distAB )
	{
		return sqrt(b.x * p.x + b.y * p.y);
	}

	// Closest point is between A and B, calc intersection point
	glm::vec2 intersection{ abNorm.x * ap.y + abNorm.y * ap.x * abNorm + glm::vec2{ a } };
	temp = glm::vec2{ p - intersection };
	return sqrt(temp.x * temp.x + temp.y * temp.y);
}

bool utils::IntersectRectLine(const Rectf& r, const glm::vec2& p1, const glm::vec2& p2, float& intersectMin, float& intersectMax)
{
	// Parameters
	// input: 
	//   r: axis aligned bounding box, start and end points of line segment.
	//   p1, p2: line
	// output: 
	//   intersectMin and intersectMax: in the interval [0,1] if intersection point is on the line segment. 
	// return
	//   true if there is an intersection

	// Example of how to use
	//float min{};
	//float max{};
	//if (utils::IntersectRectLine(rect, p1, p2, min, max))
	//{
	//	Point2f intersectP1{ p1 + (Vector2f(p2) - Vector2f(p1)) * min };
	//	Point2f intersectP2{ p1 + (Vector2f(p2) - Vector2f(p1)) * max };
	//}

	// 4 floats to convert rect space to line space
	// x1: value between 0 and 1 where 0 is on p1 and 1 is on p2, <0 and >1 means intersection is not on line segment
	float x1{ (r.left - p1.x) / (p2.x - p1.x) };
	float x2{ (r.left + r.width - p1.x) / (p2.x - p1.x) };
	float y1{ (r.bottom - p1.y) / (p2.y - p1.y) };
	float y2{ (r.bottom + r.height - p1.y) / (p2.y - p1.y) };

	using std::max; using std::min;
	float tMin{ max(min(x1,x2),min(y1,y2)) };
	float tMax{ min(max(x1,x2), max(y1,y2)) };
	if (tMin > tMax)
	{
		return false;
	}
	intersectMin = tMin;
	intersectMax = tMax;
	return true;
}

#pragma endregion CollisionFunctionality
