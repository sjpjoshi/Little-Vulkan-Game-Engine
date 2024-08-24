@echo off
REM Set the path to glslc.exe
set GLSLC=C:\VulkanSDK\1.3.283.0\Bin\glslc.exe

REM Set the path to your shader files
set SHADER_DIR=C:\Users\suraj\OneDrive\Documents\Visual Studio Projects\Little Vulkan Game Engine
REM Compile the vertex shader
"%GLSLC%" "%SHADER_DIR%\simple_shader.vert" -o "%SHADER_DIR%\simple_shader.vert.spv"

REM Compile the fragment shader
"%GLSLC%" "%SHADER_DIR%\simple_shader.frag" -o "%SHADER_DIR%\simple_shader.frag.spv"

REM Compile the vertex shader
"%GLSLC%" "%SHADER_DIR%\point_light.vert" -o "%SHADER_DIR%\point_light.vert.spv"

REM Compile the fragment shader
"%GLSLC%" "%SHADER_DIR%\point_light.frag" -o "%SHADER_DIR%\point_light.frag.spv"

echo Shader compilation complete.
pause
