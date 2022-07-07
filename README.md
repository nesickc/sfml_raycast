# What is this project?
#### This project is a demonstration of ray casting. Putting it simply, there's a "light" source on the screen that is controlled by mouse. So the source follows the cursor.
#### The source emits arbitrary (controllable) number of light beams, or rays. The rays can be blocked with walls on the scene. The walls are pre-set, but can be changed in the code.
![Alt Text](<iframe src="https://giphy.com/embed/9HJ1FoczL5SjX2J7xi" width="480" height="270" frameBorder="0" class="giphy-embed" allowFullScreen></iframe><p><a href="https://giphy.com/gifs/9HJ1FoczL5SjX2J7xi">via GIPHY</a></p>)

# What is different from the original?
1. The collision detection has been reworked so it's more clear and works better.
2. Multiple walls collision detection now work correctly (when the ray has multiple walls on its way, it stops at the closest)
3. ImGui UI has been added. It allows the user to control background color, ray color and number of rays.

# How to build?
#### You need visual studio 2019 (probably works with earlier versions)
#### Compile it for x64 Debug or Release, x86 is not supported

# Not working?
#### Check all additional dependencies in project properties:
1. `Configuration -> C/C++ -> Additional Include Directories` should point to include directory in SFML library (included with the project but you can change it to your library).
2. `Configuration -> Linker -> General-> Additional Library Directories` should point to lib directory in SFML library.
3. `Configuration -> Linker -> Input -> Additional Dependencies` should contain `sfml-system.lib`, `sfml-window.lib`, `sfml-graphics.lib` for Release configuration and `sfml-system-d.lib`, `sfml-window.lib-d`, `sfml-graphics-d.lib` for Debug configuration.

#### Note that platform should be set to x64 when setting up properties.
