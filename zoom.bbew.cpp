#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include <sstream>

using std::cout;
using std::endl;
using std::cin;

int main(int argc, char** argv)
{
        std::ofstream macro;
        std::string MacroFilename, TipsyFilename;
        std::string Macroname, Filename;
        double t, Deltat,t0,t1;
        double x, y, z, Delta1, Delta2, Deltax1, Deltay1, Deltaz1, Deltax2, Deltay2, Deltaz2;
        int Frames1, Frames2;
		
        if (argc != 5) {
            cout << "Use: movie_macro_maker <tipsymacro.macro> <tipsy.std> <dx_initial> <dx_final>" << endl;
            return 1;
		// Get commandline arguments

        MacroFilename = argv[1];
        TipsyFilename = argv[2];
		file = argv[1];


        /* Make sure that we read and write files properly */
        macro.exceptions(std::ofstream::failbit | std::ofstream::badbit);

        MacroFilename = "c-b09-v10-100k-equalmass.zoomin.macro";
        TipsyFilename = "c-b09-v10-100k-equalmass";
        //MacroFilename = "ballic-theia-100k-5000K-gamma2.new.macro";
		//TipsyFilename = "ballic-theia-100k-5000K-gamma2.new";
		//Frames = 5000;

        Macroname = "movie";

		/* Center of the box */
        x = 0.0;
        y = 0.0;
        z = 0.0;

		/* Start with 6x6x6 at t = 0 */
		Delta1 = 10.0;
        Deltax1 = Delta1; /* 40x40x40 for b > 0 */
        Deltay1 = Delta1;
        Deltaz1 = Delta1;
		Frames1 = 300;
		t0 = 0.0;

		/* End with 40x40x40 */
		Delta2 = 40.0;
        Deltax2 = Delta2; /* 40x40x40 for b > 0 */
        Deltay2 = Delta2;
        Deltaz2 = Delta2;
		Frames2 = 5000;	
		t1 = 0.6; /* Not implemented yet */

        t = 0.0;
        Deltat = 0.001; /* dt = 0.001 */

		Frames1 = 1000; /* start zoom */		
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

		/* Movie until we zoom out */
        for (int i = 0; i <= Frames1; i = i + 10)
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

            for (int j = 0; abs(j < 5 - ending.length());j++)
            {
                Filename = Filename + "0";
            }

            Filename = Filename + ending;

            t = t0 + i*Deltat;

            /* open binary file */
            macro << "openb " << Filename << endl;
            macro << "loads 0" << endl;
            macro << "redshift 0 physical 0 1 0 vacuum 1" << endl;
            macro << "xall" << endl;
            //macro << "psize gas 2" << endl;
            macro << "readarray " << Filename << ".den" << endl;
            macro << "setbox 1 " << x << " " << y << " " << z << " " << Deltax1 << " " << Deltay1 << " " << Deltaz1 << endl;
            macro << "abox 1" << endl;

            /* change this for yarray or zarray */
            //macro << "zarray gas revrain 0 40" << endl;

            //macro << "xarray gas revrain 0 15" << endl;
            macro << "zgas logtemp 0 1.5" << endl;
            //macro << "zgas temp 0 18" << endl;
		    macro << "label 790 10 7 t:" << t << endl;
            //macro << "label 790 10 7 (u:0-5) t:" << t << endl;
		    macro << "hard movie out.xwd" << endl;
            macro << "shell xwdtopnm <out.xwd | ppmtogif >" << Filename << ".gif" << endl;
            macro << "closeb" << endl;
        }

		/* Zoom out */
		for (double delta = Delta1; delta <= Delta2; delta = delta + 1)
        {
            if (delta == Delta1)
			{
					continue;
			}
			
			Filename = TipsyFilename + ".";

            std::string ending;

            std::stringstream number;

            number << Frames1;

            ending = number.str();

            for (int j = 0; abs(j < 5 - ending.length());j++)
            {
                Filename = Filename + "0";
            }

            Filename = Filename + ending;

            //t = t0 + i*Deltat;

            /* open binary file */
            macro << "openb " << Filename << endl;
            macro << "loads 0" << endl;
            macro << "redshift 0 physical 0 1 0 vacuum 1" << endl;
            macro << "xall" << endl;
            //macro << "psize gas 2" << endl;
            macro << "readarray " << Filename << ".den" << endl;
            macro << "setbox 1 " << x << " " << y << " " << z << " " << delta << " " << delta << " " << delta << endl;
            macro << "abox 1" << endl;

            /* change this for yarray or zarray */
            //macro << "zarray gas revrain 0 40" << endl;

            //macro << "xarray gas revrain 0 15" << endl;
            macro << "zgas logtemp 0 1.5" << endl;
            //macro << "zgas temp 0 18" << endl;
		    macro << "label 790 10 7 t:" << t << endl;
            //macro << "label 790 10 7 (u:0-5) t:" << t << endl;
		    macro << "hard movie out.xwd" << endl;
            macro << "shell xwdtopnm <out.xwd | ppmtogif >" << Filename << "." << delta <<  ".zoom.gif" << endl;
            macro << "closeb" << endl;
        }
		/* Continue until t = t2 */
		for (int i = Frames1; i <= Frames2; i = i + 10)
        {
            if (i == 0 || i == Frames1)
			{
					continue;
			}
			
			Filename = TipsyFilename + ".";

            std::string ending;

            std::stringstream number;

            number << i;

            ending = number.str();

            for (int j = 0; abs(j < 5 - ending.length());j++)
            {
                Filename = Filename + "0";
            }

            Filename = Filename + ending;

            t = t0 + i*Deltat;

            /* open binary file */
            macro << "openb " << Filename << endl;
            macro << "loads 0" << endl;
            macro << "redshift 0 physical 0 1 0 vacuum 1" << endl;
            macro << "xall" << endl;
            //macro << "psize gas 2" << endl;
            macro << "readarray " << Filename << ".den" << endl;
            macro << "setbox 1 " << x << " " << y << " " << z << " " << Deltax2 << " " << Deltay2 << " " << Deltaz2 << endl;
            macro << "abox 1" << endl;

            /* change this for yarray or zarray */
            //macro << "zarray gas revrain 0 40" << endl;

            //macro << "xarray gas revrain 0 15" << endl;
            macro << "zgas logtemp 0 1.5" << endl;
            //macro << "zgas temp 0 18" << endl;
		    macro << "label 790 10 7 t:" << t << endl;
            //macro << "label 790 10 7 (u:0-5) t:" << t << endl;
		    macro << "hard movie out.xwd" << endl;
            macro << "shell xwdtopnm <out.xwd | ppmtogif >" << Filename << ".gif" << endl;
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
