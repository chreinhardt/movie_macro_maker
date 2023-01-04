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
        double t, dt, Delta;
        double x0, y0, z0, Delta1, Delta2;
        int iFrame;
		
        if (argc != 6) {
            cout << "Use: movie_macro_maker <tipsymacro.macro> <tipsy.std> <frame> <dx_initial> <dx_final>" << endl;
            return 1;
		}

		// Get commandline arguments
        MacroFilename = argv[1];
        TipsyFilename = argv[2];
		iFrame = atoi(argv[3]);
        Delta1 = atof(argv[4]);
        Delta2 = atof(argv[5]);

        /* Make sure that we read and write files properly */
        macro.exceptions(std::ofstream::failbit | std::ofstream::badbit);

        Macroname = "movie";

		/* Center of the box */
        x0 = 0.0;
        y0 = 0.0;
        z0 = 0.0;

		dt = 0.001;
		t = iFrame*Delta;
        
		sprintf(Filename.c_str(),"%s.%.5i",TipsyFilename.c_str(),iFrame);

		printf("%s %s %s %i %g %g\n",Filename.c_str(),TipsyFilename.c_str(),iFrame,Delta1,Delta2);
		
		exit(1);

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
		macro << "psize 0" << endl;

		Delta = Delta1;

		/* Zoom out */
		while (Delta <= Delta2)
        {
            if (Delta == Delta1)
			{
					continue;
			}

/*	
			Filename = TipsyFilename + ".";

            std::string ending;

            std::stringstream number;

            number << iFrame;

            ending = number.str();

            for (int j = 0; abs(j < 5 - ending.length());j++)
            {
                Filename = Filename + "0";
            }

            Filename = Filename + ending;
*/

            /* open binary file */
            macro << "openb " << Filename << endl;
            macro << "loads 0" << endl;
            macro << "redshift 0 physical 0 1 0 vacuum 1" << endl;
            macro << "xall" << endl;
            //macro << "psize gas 2" << endl;
            macro << "readarray " << Filename << ".den" << endl;
            macro << "setbox 1 " << x0 << " " << y0 << " " << z0 << " " << Delta << " " << Delta << " " << Delta << endl;
            macro << "abox 1" << endl;

            /* change this for yarray or zarray */
            //macro << "zarray gas revrain 0 40" << endl;

            //macro << "xarray gas revrain 0 15" << endl;
            macro << "zgas logtemp 0 1.5" << endl;
            //macro << "zgas temp 0 18" << endl;
		    macro << "label 790 10 7 t:" << t << endl;
            //macro << "label 790 10 7 (u:0-5) t:" << t << endl;
		    macro << "hard movie out.xwd" << endl;
            macro << "shell xwdtopnm <out.xwd | ppmtogif >" << Filename << "." << Delta <<  ".zoom.gif" << endl;
            macro << "closeb" << endl;
			Delta += 1.0;
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
