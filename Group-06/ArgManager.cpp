
#include <iostream>
#include <vector>

namespace cse {

    class ArgManager {
    public:
        ArgManager(int argc, char *argv[])
        {
            //Load in all the command line options
            LoadArgManager();

            //pass argc and argv
            _argc = argc;
            for (int i = 0; i < argc; i++)
            {
                _argv.push_back(argv[i]);
            }
        }

        void LoadArgManager()
        {
            
        }
        /*void ConvertArg(int argc, char *argv[])
        {

        }
        void ListOptions()
        {

        }*/

        bool Has(std::string argv)
        {
            for (int i = 0 ; i < _argc; i++)
            {
                if (_argv[i] == argv)
                {
                    return true;
                }
            }
        }

        std::string GetOption(std::string argv)
        {
            for (int i = 0 ; i < _argc; i++)
            {
                if (_argv[i] == argv)
                {
                    return _argv[i + 1];
                }
            }
        }

    private:

        //store args
        int _argc = 0;
        std::vector<std::string> _argv;
    };
    void PrintHelp()
    {
        
    }

    int main(int argc, char *argv[]) {
        ArgManager args(argc, argv);  // Pass input args into manager for processing.
        if (args.Has("-h") || args.Has("--help")) {
            PrintHelp();
            exit(0);
        }
        // Set a default output filename, then Test if one was specified in the
        // command line arguments sent in.
        std::string out_filename="default_output.dat";
        if (args.Has("-o")) out_filename = args.GetOption("-o");
    }
}
