#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <iomanip>

using std::cout;
using std::endl;
using std::cin;

int main(int argc, char** argv)
{
        std::ofstream macro;
        std::string MacroFilename, TipsyFilename;
        std::string Macroname, Filename;
        double t, Deltat,t0;
        double x, y, z, Deltax, Deltay, Deltaz;
        double v_com_x, v_com_y, v_com_z;
        int Frames, DeltaFrames;
        int nDigits;
        double time_unit = 1.77;

        if (argc != 15) {
            cout << "Use: movie_macro_maker <macrofile> <tipsyfiles> <x> <y> <z> <vx> <vy> <vz> <dx> <dy> <dz> <Frames> <n Frames to skip inbetween>  <dt>" << endl;
            return 1;
        } 
		
		MacroFilename = argv[1];
		TipsyFilename =  argv[2];

        x = atof(argv[3]);
        y = atof(argv[4]);
        z = atof(argv[5]);

        v_com_x = atof(argv[6]);
        v_com_y = atof(argv[7]);
        v_com_z = atof(argv[8]);

        Deltax = atof(argv[9]);
        Deltay = atof(argv[10]);
        Deltaz = atof(argv[11]);

		Frames = atoi(argv[12]);
        DeltaFrames = atoi(argv[13]);
        Deltat = atof(argv[14]);

        /* Make sure that we read and write files properly */
        macro.exceptions(std::ofstream::failbit | std::ofstream::badbit);

        Macroname = "movie";

        /* Start at to t=0. */
		t0 = 0.0;
        t = t0;

        /*
        if (Frames < 1e5)
        {
            nDigits = 5;
        } else if (Frames < 1e6) {
            nDigits = 6;
        } else {
            cout << "Could not determine nDigits!\n";
            exit(1);
        }
        */

        /* Open macro file */
        try {
            macro.open(MacroFilename.c_str());
        }
        catch (const std::ofstream::failure& e)
        {
            cout << "Error while opening/reading file: " << e.what() << endl;
            return 1;
        }

        macro << Macroname << endl;

        for (int i = 0; i <= Frames; i = i + DeltaFrames)
        {
            if (i == 0)
			{
					continue;
			}
			
			Filename = TipsyFilename + ".";

            std::string ending;

            std::stringstream number;

            number << i;

            ending = number.str();
        
            if (i < 100000)
            {
                nDigits = 5;
            } else if (i < 1000000) {
                nDigits = 6;
            } else {
                cout << "Could not determine nDigits!\n";
                exit(1);
            }

            for (int j = 0; abs(j < nDigits - ending.length());j++)
            {
                Filename = Filename + "0";
            }

            Filename = Filename + ending;
            
            t = t0 + i*Deltat;

            /* Move COM according to r_com = v_com*dt + r_com_0 */
            x += v_com_x*DeltaFrames*Deltat;
            y += v_com_y*DeltaFrames*Deltat;
            z += v_com_z*DeltaFrames*Deltat;

            /* open binary file */
            macro << "openb " << Filename << endl;
   			macro << "loads 0" << endl;
            macro << "redshift 0 physical 0 1 0 vacuum 1" << endl;
            macro << "xall" << endl;
			macro << "readmark Particle40687.mark" << endl;

            macro << "psize gas 0" << endl;
            macro << "readarray " << Filename << ".den" << endl;
            macro << "setbox 1 " << x << " " << y << " " << z << " " << Deltax << " " << Deltay << " " << Deltaz << endl;
            macro << "abox 1" << endl;

            /* change this for yarray or zarray */
            macro << "zarray gas revrain 0 18" << endl;
			
			//macro << "xall" << endl;
            //macro << "xarray gas revrain 0 12" << endl;
            //macro << "zgas logtemp 0 1.5" << endl;
            //macro << "zgas temp 0 5" << endl;
		    macro << "label 790 10 7 t:" << std::setprecision(3) << (t*time_unit) << "h" << endl;
            //macro << "label 790 10 7 (u:0-5) t:" << t << endl;
		    macro << "hard movie out.xwd" << endl;
            macro << "shell xwdtopnm <out.xwd | pnmtopng >" << Filename << ".png" << endl;
            macro << "closeb" << endl;
        }

        macro << "end" << endl;

        /* Close macro file */
        try {
            macro.close();
        }
        catch (const std::ofstream::failure& e)
        {
            cout << "Error while opening/reading file: " << e.what() << endl;
            return 1;
        }

        cout << "Done." << endl;
}
