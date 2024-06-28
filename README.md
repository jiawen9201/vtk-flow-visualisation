# vtk-flow-visualisation
 
To run the code:
1. Download the flowVisSample folder as a zip file and extract to computer on desired location.

2. Create a new folder named "build" inside the flowVisSample folder.

3. Open CMake, set the source code location to the flowVisSample folder and the location to build the binaries to the build folder, then click configure. Make sure to set the VTK_DIR to the path of the installed VTK bin. After configure, click generate to generate the project file.

4. Go to build folder, open the flowVisSample.sln using Visual Studio.

5. On the solution explorer, right mouseclick the flowVisSample and select properties. Make sure the **Environment** inside **Configuration Properties -> Debugging** is set to **"path=your-path-to-vtk\bin\bin\Debug"**, the **Include Directories** and **Library Directories** inside **Configuration Properties -> VC++ Directories** is set to **"your-path-to-vtk\install\include\vtk-9.3;$(IncludePath)"** and **"your-path-to-vtk\install\lib;$(LibraryPath)"** respectively, and the **Additional Dependencies** inside **Linker -> Input** is set to the content of the **LIST.TXT** file inside the folder **"your-path-to-vtk\install\lib"** during the installation of vtk.

6. After setting, right click the flowVisSample, select Debug and choose Start New Instance or use Local Window Debugger to run the program.

7. The initial content inside the flowVisSample.cxx is the content of the hedgehog.cxx, if want to run another cxx file, copy the content of the cxx file and paste inside flowVisSample.cxx.

8. All the code to read different dataset is provided and commented, uncomment or comment the lines belongs to the dataset to visualise the dataset.
