#include "vtkRenderer.h"
#include "vtkPointSet.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkVolume16Reader.h"
#include "vtkPolyDataMapper.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "vtkOutlineFilter.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
#include "vtkPolyDataNormals.h"
#include "vtkContourFilter.h"
#include "vtkStructuredPointsReader.h"
#include "vtkMarchingCubes.h"
#include "vtkRecursiveDividingCubes.h"
#include "vtkScalarBarWidget.h"
#include "vtkScalarBarActor.h"
#include "vtkPiecewiseFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkVolumeProperty.h"
//#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
//#include "vtkVolumeRayCastMIPFunction.h"
#include "vtkStructuredPointsReader.h"
#include "vtkThresholdPoints.h"
#include "vtkMaskPoints.h"
#include "vtkConeSource.h"
#include "vtkGlyph3D.h"
#include "vtkLookupTable.h"
#include "vtkCommand.h"
#include "vtkHedgeHog.h"
#include "vtkStreamTracer.h"
#include "vtkHyperStreamline.h"
#include "vtkPoints.h"
#include "vtkTransform.h"
#include "vtkTransformFilter.h"

int main(int argc, char** argv)
{
	// Create the renderer, the render window, and the interactor. The renderer
	// draws into the render window, the interactor enables mouse- and 
	// keyboard-based interaction with the data within the render window.
	vtkRenderer* aRenderer = vtkRenderer::New();
	vtkRenderWindow* renWin = vtkRenderWindow::New();

	renWin->AddRenderer(aRenderer);
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	// Read
	vtkStructuredPointsReader* reader = vtkStructuredPointsReader::New();
	//reader->SetFileName("../data/testData1.vtk");
	//reader->SetFileName("../data/testData2.vtk");
	reader->SetFileName("../data/carotid.vtk");
	reader->Update();

	// for carotid need to transform the origin to render the data correctly 
	double origin[3] = { 100.0, 80.0, 1.0 };
	vtkTransform* transform = vtkTransform::New();
	transform->Translate(-origin[0], -origin[1], -origin[2]);

	vtkSmartPointer<vtkTransformFilter> transformedReader = vtkSmartPointer<vtkTransformFilter>::New();
	transformedReader->SetInputConnection(reader->GetOutputPort());
	transformedReader->SetTransform(transform);
	transformedReader->Update();

	// Lookup Table
	vtkLookupTable* lut = vtkLookupTable::New();
	lut->SetHueRange(0.667, 0.0);
	lut->Build();

	vtkStreamTracer* streamline1 = vtkStreamTracer::New();

	vtkPointSet* seedPoint = vtkPointSet::New();
	vtkPoints* points = vtkPoints::New();

	// Define the grid parameters

	// testData1
	/*
	int xSize = 10;  // Number of points in x-direction
	int ySize = 10;  // Number of points in y-direction
	int zSize = 2;  // Number of points in z-direction
	*/

	// testData2
	/*
	int xSize = 30;  // Number of points in x-direction
	int ySize = 30;  // Number of points in y-direction
	int zSize = 2;  // Number of points in z-direction
	*/

	// carotid
	int xSize = 3;  // Number of points in x-direction
	int ySize = 3;  // Number of points in y-direction
	int zSize = 3;  // Number of points in z-direction


	// testData1
	/*
	int maxX = 36;
	int minX = 0;
	int maxY = 36;
	int minY = 0;
	int maxZ = 0;
	int minZ = 0;
	double maxProg = 100;
	double step = 0.01;
	*/

	// testData2
	/*
	int maxX = 357;
	int minX = 0;
	int maxY = 357;
	int minY = 0;
	int maxZ = 0;
	int minZ = 0;
	double maxProg = 100;
	double step = 0.1;
	*/

	// carotid
	int maxX = 50;
	int minX = 26;
	int maxY = 49;
	int minY = 0;
	int maxZ = 35;
	int minZ = 10;
	double maxProg = 1000;
	double step = 0.1;

	// Compute the spacing between the grid points
	double xSpacing = (static_cast<double>(maxX) - minX) / (xSize - 1);
	double ySpacing = (static_cast<double>(maxY) - minY) / (ySize - 1);
	double zSpacing = (static_cast<double>(maxZ) - minZ) / (zSize - 1);

	// Generate the grid of seed points
	for (int k = 0; k < zSize; k++) {
		for (int j = 0; j < ySize; j++) {
			for (int i = 0; i < xSize; i++) {
				double point[3];
				point[0] = minX + i * xSpacing;
				point[1] = minY + j * ySpacing;
				point[2] = minZ + k * zSpacing;
				points->InsertNextPoint(point);
			}
		}
	}

	seedPoint->SetPoints(points);

	vtkPolyData* polyData = vtkPolyData::New();
	polyData->SetPoints(points);

	//streamline1->SetInputConnection(reader->GetOutputPort()); // for testData1 & testData2
	streamline1->SetInputConnection(transformedReader->GetOutputPort()); // for carotid
	streamline1->SetSourceData(polyData);
	streamline1->SetIntegrationDirectionToBoth();
	streamline1->SetMaximumPropagation(maxProg);
	streamline1->SetIntegrationStepUnit(step);
	streamline1->Update();

	// Streamline visualization
	vtkPolyDataMapper* streamlineMapper = vtkPolyDataMapper::New();
	streamlineMapper->SetInputConnection(streamline1->GetOutputPort());
	streamlineMapper->ScalarVisibilityOff();

	vtkActor* streamlineActor = vtkActor::New();
	streamlineActor->SetMapper(streamlineMapper);
	streamlineActor->GetProperty()->SetColor(1.0, 0.0, 0.0); // Set color to red

	aRenderer->AddActor(streamlineActor);

	// Set a background color for the renderer and set the size of the
	// render window (expressed in pixels).
	aRenderer->SetBackground(0, 0, 0);
	renWin->SetSize(800, 600);

	// Initialize the event loop and then start it.
	iren->Initialize();
	renWin->SetWindowName("Streamline");
	renWin->Render();
	iren->Start();

	return 0;
}
