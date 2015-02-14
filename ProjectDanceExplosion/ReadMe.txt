
DON'T DO THIS UNTIL YOU'VE TRIED TO RUN THE PROJECT

Possible Setup(if you get errors, do this)

-------- Possible Fix Number 1 -----------
View -> Solution Explorer

There should be a button "Show All Files"
If you can't see it, extend your Solution Explorer drawer
and it should appear

If any file (except this one "Read me") appears with a red 'x'
next to it, right click and select "Include in project"

Build project, then run project

-------- Possible Fix Number 2 -----------
With project open:
Tools -> NuGet Package Management -> Package Manager Console

Taken from Nuget Website:
"When you use NuGet to install a package, it copies the library
files to your solution and automatically updates your project
(add references, change config files, etc.)."
If necessary, it will also download the libraries you request.

These are the libraries we need for our project,
so enter the following one at a time:

	PM> Install-Package boost
            Install-Package assimp
	    Install-Package glew
	    Install-Package freeglut
	    Install-Package glm

Some of these are a little slow at giving feedback,
To check all libraries have been installed
, open your project's "packages.config" file.
By the end of all the above, it should look like this.

	<?xml version="1.0" encoding="utf-8"?>
	<packages>
	  <package id="Assimp" version="3.0.0" targetFramework="Native" />
	  <package id="Assimp.redist" version="3.0.0" targetFramework="Native" />
	  <package id="boost" version="1.57.0.0" targetFramework="Native" />
	  <package id="freeglut" version="2.8.1.15" targetFramework="Native" />
	  <package id="freeglut.redist" version="2.8.1.15" targetFramework="Native" />
	  <package id="glew" version="1.9.0.1" targetFramework="Native" />
	  <package id="glew.redist" version="1.9.0.1" targetFramework="Native" />
	  <package id="glm" version="0.9.6.1" targetFramework="Native" />
	</packages>

Assimp = our model loader
it relies on c++ boost libraries so they're included as well

OpenGL
As far as I can see, this cuts out all the linking and crap
 if doesn't work we'll fall back on what we know

GLM
I have no idea how you guys got GLM into your projects,
this is how my tutorial did it.
---------------------------------------------------------------

CODE STRUCTURE

Main
 - all OpenGL stuff, renderScene method, camera setup, keyboard control

ShaderLoader
 - our GLSL compiler-type thing, taken from http://in2gpu.com/2014/10/29/shaders-basics/

Mesh
 - pulls data out of aiMesh and stores it in a better format

			Main
			  |
		----------------------
	ShaderLoader		    Mesh
---------------------------------------------------------------
Drawing Models Explained

Assimp loads a file as an aiScene type.
This aiScene contains multiple aiMeshes.
aiMeshes contain vertices, normals, texture coordinates,
 a skeleton and other fun stuff.
So for Nanosuit, Assimp would have 1 aiScene with 10 aiMeshes.
The aiMeshes contain all the data we need to draw a model.
So here's how we do that:

Main::LoadModel
 For every aiMesh in scene create a Mesh object
 This Mesh object is stored in <vector>modelList
  Mesh object created using Mesh::Mesh(aiMesh* m) -> see below
 The number of aiMeshs in aiScene is stored in numMeshes

Main::RenderScene()
 Creates 1 MVP
 Creates 1 vao
 For loop moves though <vector>modelList
	creates unique vbo for each model in modelList
	buffers vertex data in vbo
	 vertex data obtained using Mesh::GetVertexData()
	passes vbo into shaders
	draws vbo (ya don't say?)

Mesh::Mesh(aiMesh* m)
 When a Mesh is created:
	its number of vertices is calculated
	its number of faces is stored
	its vertesData is stored in an understandable format

Mesh::GetVertexData()
 Returns a pointer to the first element of vertexData vector
 Apparantly this is enough to buffer all the data in the vector
  - I shall not argue, it's incredibly handy!




------------------------------------------------------CLARKES OBSERVATIONS---------------------------------------------------------

I've been looking at how assimp structures the imported model and have been experimenting with various things


- EDIT: Bones are present in the model. The model I was looking at before just didn't have any.

- the aiMesh class holds the vertices and the bones (aiBone), 
- As well as various other things (textures etc. but that's not important for the moment).

- the bone holds the transformation and the nodes that are affected by it

- we need to pass the vertices in (as we already do), then transform them using the appropriate bone

- My current thoughts are:
	- Create an array the size of the vertices that contains the transformations for each vertex
		- Probably VERY slow and messy
	
	- Pass the transformation array in, as well as the bones affected and see which transformation the vertex should use
		- for example, vertices 100-200 are affected by bone 1
		- we have a uniform counter that ++s every iteration of the shader (don't think this is possible)
		- there's a loop that goes through the bone list and sees where the current vertex applies
		- it transforms the vertex by the bone's matrix

	- A combination: pass in the array of bones and the bone Index that we should use for that vertex, then transform it
		- so we have our array of floats for the vetices and an array of ints for the bon indices and pass them in
		- we also pass in the bone array, as a Uniform variable, and just transform the vertex by bone[index]
		- I like the sound of this one best.
		- the array of indices only needs to be created initially, and doesn't need to be updated
		- the bones are the only things that need updating actually, as we shouldn't be transforming the vertices every frame.