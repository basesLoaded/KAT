#pragma once

#include <getopt.h>
#include <string.h>
#include <stdint.h>
#include <iostream>

using std::string;
using std::cerr;
using std::cout;
using std::endl;


const string DEFAULT_TITLE      = "Title";
const string DEFAULT_X_LABEL    = "X";
const string DEFAULT_Y_LABEL    = "Y";
const string DEFAULT_Z_LABEL    = "Z";
const int64_t DEFAULT_Z_CAP     = -1;
const uint16_t DEFAULT_WIDTH    = 1024;
const uint16_t DEFAULT_HEIGHT   = 1024;
const int32_t DEFAULT_X_MAX     = -1;
const int32_t DEFAULT_Y_MAX     = -1;


class FlamePlotArgs
{
public:
    string*     mx_arg;
    string*     output_type;
    string*     output_path;
    string      title;
    string      x_label;
    string      y_label;
    string      z_label;
    int16_t     x_max;
    int16_t     y_max;
    uint16_t    width;
    uint16_t    height;
    int64_t     z_cap;
    bool        verbose;

    // Default constructor
    FlamePlotArgs() :
        mx_arg(NULL), output_type(NULL), output_path(NULL), title(DEFAULT_TITLE), x_label(DEFAULT_X_LABEL), y_label(DEFAULT_Y_LABEL), z_label(DEFAULT_Z_LABEL),
        x_max(DEFAULT_X_MAX), y_max(DEFAULT_Y_MAX), width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT), z_cap(DEFAULT_Z_CAP), verbose(false)
    {
        output_type = new string("png");
    }

    // Constructor that parses command line options
    FlamePlotArgs(int argc, char* argv[]) :
        mx_arg(NULL), output_type(NULL), output_path(NULL), title(DEFAULT_TITLE), x_label(DEFAULT_X_LABEL), y_label(DEFAULT_Y_LABEL), z_label(DEFAULT_Z_LABEL),
        x_max(DEFAULT_X_MAX), y_max(DEFAULT_Y_MAX), width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT), z_cap(DEFAULT_Z_CAP), verbose(false)
    {
        output_type = new string("png");
        parse(argc, argv);
    }

    ~FlamePlotArgs()
    {
        delete mx_arg;
        delete output_type;
        delete output_path;
    }


#define flame_plot_args_USAGE "Usage: kat plot flame [options] -o <output_file_path> matrix_path\n"
    const char * usage() const
    {
        return flame_plot_args_USAGE;
    }

    void error(const char *msg)
    {
        cerr << endl
             << "Error: " << msg << endl << endl
             << usage() << endl
             << "Use --help for more information" << endl;
        exit(1);
    }


#define flame_plot_args_HELP "Create Kmer Flame Plots\n\n" \
  "  Creates a flame plot from a matrix created with the \"comp\" tool.  Typically this\n" \
  "  is used to compare two kmer hashes produced by different NGS reads.\n\n" \
  "Options (default value in (), *required):\n" \
  " -p, --output_type    The plot file type to create: png, ps, pdf.  Warning... if pdf is selected\n" \
  "                      please ensure your gnuplot installation can export pdf files. (png)\n" \
  " -o, --output         Output file (<matrix_path>.<output_type>)\n" \
  " -t, --title          Title for plot\n" \
  " -i, --x_label        Label for the x-axis\n" \
  " -j, --y_label        Label for the y-axis\n" \
  " -k, --z_label        Label for the z-axis\n" \
  " -x, --x_max          Maximum value for the x-axis\n" \
  " -y  --y_max          Maximum value for the y-axis\n" \
  " -w, --width          Width of canvas (1024)\n" \
  " -h, --height         Height of canvas (1024)\n" \
  " -z, --z_cap          Cap for matrix values.  Values greater than this cap will be displayed at maximum intensity, i.e. white. (10000)\n" \
  " -v, --verbose        Outputs additional information to stderr\n" \
  "     --usage          Usage\n" \
  "     --help           This message\n"

    const char * help() const
    {
        return flame_plot_args_HELP;
    }

#define flame_plot_args_HIDDEN "Hidden options:"
    const char * hidden() const
    {
        return flame_plot_args_HIDDEN;
    }


    void parse(int argc, char *argv[])
    {
        int c;
        int help_flag = 0;
        int usage_flag = 0;

        static struct option long_options[] =
        {
            {"verbose",         no_argument,        0, 'v'},
            {"output_type",     required_argument,  0, 'p'},
            {"output",          required_argument,  0, 'o'},
            {"title",           required_argument,  0, 't'},
            {"x_label",         required_argument,  0, 'i'},
            {"y_label",         required_argument,  0, 'j'},
            {"z_label",         required_argument,  0, 'k'},
            {"x_max",           required_argument,  0, 'x'},
            {"y_max",           required_argument,  0, 'y'},
            {"width",           required_argument,  0, 'w'},
            {"height",          required_argument,  0, 'h'},
            {"z_cap",           required_argument,  0, 'z'},
            {"help",            no_argument,        &help_flag, 1},
            {"usage",           no_argument,        &usage_flag, 1},
            {0, 0, 0, 0}
        };

        static const char *short_options = "o:p:t:i:j:k:x:y:w:h:z:vuh";

        if (argc <= 1)
        {
            cerr << endl
                 << usage() << endl
                 << help() << endl;
            exit(1);
        }

        while (true)
        {
            /* getopt_long stores the option index here. */
            int index = -1;

            c = getopt_long (argc, argv, short_options, long_options, &index);


            /* Detect the end of the options. */
            if (c == -1)
                break;

            switch (c)
            {
            case ':':
                cerr << "Missing required argument for "
                          << (index == -1 ? std::string(1, (char)optopt) : std::string(long_options[index].name))
                          << endl << endl;
                exit(1);
            case '?':
                cerr << "Use --usage or --help for some help" << endl << endl;
                exit(1);
            case 'v':
                verbose = true;
                break;
            case 'o':
                output_path = new string(optarg);
                break;
            case 'p':
                delete output_type;
                output_type = new string(optarg);
                break;
            case 't':
                title = string(optarg);
                break;
            case 'i':
                x_label = string(optarg);
                break;
            case 'j':
                y_label = string(optarg);
                break;
            case 'k':
                z_label = string(optarg);
                break;
            case 'x':
                x_max = atoi(optarg);
                break;
            case 'y':
                y_max = atoi(optarg);
                break;
            case 'w':
                width = atoi(optarg);
                break;
            case 'h':
                height = atoi(optarg);
                break;
            case 'z':
                z_cap = atoi(optarg);
                break;
            }
        }

        if (help_flag)
        {
            cout << usage() << endl
                 << help() << endl;
            exit(0);
        }

        if (usage_flag)
        {
            cout << usage() << endl
                 << "Use --help for more information." << endl << endl;
            exit(0);
        }

        // Parse arguments
        if(argc - optind != 1)
            error("Requires exactly 1 argument.");
        mx_arg = new string(argv[optind++]);
    }

    // Work out the output path to use (either user specified or auto generated)
    string determineOutputPath()
    {
        std::ostringstream output_str;
        output_str << *mx_arg << "." << *output_type;
        return output_path == NULL ? output_str.str() : *output_path;
    }


    void print()
    {
        if (verbose)
            cerr << "Verbose flag set\n";

        if (output_type != NULL)
            cerr << "Output type: " << *output_type << endl;

        if (output_path != NULL)
            cerr << "Output file specified: " << *output_path << endl;

        if (mx_arg != NULL)
            cerr << "Kmer Matrix input file specified: " << *mx_arg << endl;

        if (!title.empty())
            cerr << "Plot title: " << title << endl;

        if (!x_label.empty())
            cerr << "X Label: " << x_label << endl;

        if (!y_label.empty())
            cerr << "Y Label: " << y_label << endl;

        if (!z_label.empty())
            cerr << "Z Label: " << z_label << endl;

        if (x_max)
            cerr << "X Max: " << x_max << endl;

        if (y_max)
            cerr << "Y Max: " << y_max << endl;

        if (width)
            cerr << "Width: " << width << endl;

        if (height)
            cerr << "Height: " << height << endl;

        if (z_cap)
            cerr << "Z cap: " << z_cap << endl;

        cerr << endl;
    }
};

