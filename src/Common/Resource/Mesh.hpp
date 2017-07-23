#ifndef MESH_HPP
#define MESH_HPP

//Prototype Class
class ShaderProgram;

class Mesh
{
    public:
		virtual void draw(ShaderProgram* program) = 0;
};

#endif // Mesh_HPP
