
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