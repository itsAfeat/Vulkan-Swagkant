@echo off

del *.spv

echo Compiling vertex shaders...
for %%f in (shaders/*.vert) do glslc.exe shaders/%%f -o %%f.spv
echo Compiling fragment shaders...
for %%f in (shaders/*.frag) do glslc.exe shaders/%%f -o %%f.spv
