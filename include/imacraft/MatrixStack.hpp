#ifndef _MATRIXSTACK_HPP_
#define _MATRIXSTACK_HPP_

#include <stack>
#include <glm/glm.hpp>

class MatrixStack{
	private :
		std::stack<glm::mat4> m_Stack;
		
	public :
		MatrixStack();
		~MatrixStack();
		void push();
		void pop();
		void mult(const glm::mat4& m);
		const glm::mat4& top() const;
		void set(const glm::mat4& m);
		void scale(const glm::vec3& s);
		void translate(const glm::vec3& t);
		void rotate(float degreed, const glm::vec3& r);
};


#endif // _MATRIXSTACK_HPP_
