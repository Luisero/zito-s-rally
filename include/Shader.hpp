#pragma once

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader {
public:
    
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    
    ~Shader();

   
    void use() const;

 
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
   
    void checkCompileErrors(unsigned int shader, std::string type);
};