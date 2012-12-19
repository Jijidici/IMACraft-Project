#include "imacraft/MatrixStack.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

MatrixStack::MatrixStack(){
	this->m_Stack.push(glm::mat4(1.f));
}

MatrixStack::~MatrixStack(){
}

void MatrixStack::push(){
	this->m_Stack.push(m_Stack.top());
}

void MatrixStack::pop(){
	this->m_Stack.pop();
}

void MatrixStack::mult(const glm::mat4& m){
	this->m_Stack.top() = m_Stack.top() * m;
}

const glm::mat4& MatrixStack::top() const{
	return this->m_Stack.top();
}

void MatrixStack::set(const glm::mat4& m){
	this->m_Stack.top() = m;
}

void MatrixStack::scale(const glm::vec3& s){
	this->m_Stack.top() = glm::scale(m_Stack.top(), s);
}

void MatrixStack::translate(const glm::vec3& t){
	this->m_Stack.top() = glm::translate(m_Stack.top(), t);
}

void MatrixStack::rotate(float degrees, const glm::vec3& r){
	this->m_Stack.top() = glm::rotate(m_Stack.top(), degrees, r);
}
