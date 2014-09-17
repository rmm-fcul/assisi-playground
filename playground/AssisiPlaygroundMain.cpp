#include <QApplication>
#include <QImage>

#include <boost/program_options.hpp>

#include "WorldExt.h"
#include "AssisiPlayground.h"

#include "extensions/ExtendedWorld.h"
#include "interactions/WorldHeat.h"

#include "handlers/PhysicalObjectHandler.h"
#include "handlers/EPuckHandler.h"
#include "handlers/CasuHandler.h"
#include "handlers/BeeHandler.h"

#include "robots/Bee.h"
#include "robots/Casu.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace Enki;

namespace po = boost::program_options;

namespace Enki {
	double env_temp;
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
    /** Parse command line options **/
    
    po::options_description desc("Recognized options");
    
    // Variables to store the options
    int r;
    string config_file_name("Playground.cfg");
    double heat_scale;
    int heat_border_size;

    double maxHeat;
    double maxVibration;

    desc.add_options
        ()
        ("help,h", "produce help message")
        ("config_file,c", 
         po::value<string>(&config_file_name)->default_value("Playground.cfg"),
         "configuration file name")
        ("Arena.radius,r", po::value<int>(&r), 
         "playground radius, in cm")
        ("Heat.env_temp,t", po::value<double>(&env_temp), 
         "environment temperature, in C")
        ("Heat.scale", po::value<double>(&heat_scale), 
         "heat model scale")
        ("Heat.border_size", po::value<int>(&heat_border_size), "playground radius, in cm")
         (
          "Vibration.range",
          po::value<double> (&Casu::VIBRATION_SOURCE_RANGE),
          "vibration range, in cm"
          )
        (
         "Vibration.maximum_amplitude", 
         po::value<double> (&Casu::VIBRATION_SOURCE_MAXIMUM_AMPLITUDE),
         "maximum amplitude of vibration"
         )
        (
         "Vibration.amplitude_quadratic_decay", 
         po::value<double> (&Casu::VIBRATION_SOURCE_AMPLITUDE_QUADRATIC_DECAY),
         "quadratic decay of vibration amplitude"
         )
        (
         "Vibration.noise", 
         po::value<double> (&Casu::VIBRATION_SOURCE_NOISE),
         "vibration frequency noise"
         )
        (
         "Viewer.max_heat",
         po::value<double> (&maxHeat),
         "maximum displayed heat"
         )
        (
         "Viewer.max_vibration",
         po::value<double> (&maxVibration),
         "maximum displayed vibration intensity"
          )
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    ifstream config_file(config_file_name.c_str(), std::ifstream::in);
    po::store(po::parse_config_file(config_file, desc), vm);
    config_file.close();
    po::notify(vm);

    if (vm.count("help"))
    {
        cout << desc << endl;
        return 1;
    }

	// Create the world and the viewer
    string pub_address("tcp://*:5555"); 
    string sub_address("tcp://*:5556");

    //QImage texture("playground/world.png");
    QImage texture(QString(":/textures/ground_grayscale.png"));
    texture = QGLWidget::convertToGLFormat(texture);    
    //texture.invertPixels(QImage::InvertRgba);
    
    WorldExt world (r, pub_address, sub_address,
		Color::gray, 
		World::GroundTexture (
			texture.width(),
			texture.height(),
			(const uint32_t*) texture.constBits ()));

	WorldHeat *heatModel = new WorldHeat(env_temp, heat_scale, heat_border_size);
	world.addPhysicSimulation(heatModel);

	CasuHandler *ch = new CasuHandler();
	world.addHandler("Casu", ch);

	PhysicalObjectHandler *ph = new PhysicalObjectHandler();
	world.addHandler("Physical", ph);

	BeeHandler *bh = new BeeHandler();
	world.addHandler("Bee", bh);

	AssisiPlayground viewer (&world, heatModel, maxHeat, maxVibration);	
	viewer.show ();
	
	return app.exec();

}

