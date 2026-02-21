#include "tbox/tbox.h"

// A simple audio application using TBOX
// This example shows how to use TBOX for basic audio processing tasks

static tb_void_t print_usage(tb_char_t const* program)
{
    tb_trace_i("usage: %s [options]", program);
    tb_trace_i("options:");
    tb_trace_i("  -h, --help     Show this help message");
    tb_trace_i("  -f, --file     Audio file to process");
    tb_trace_i("  -v, --verbose  Verbose output");
    tb_trace_i("");
    tb_trace_i("examples:");
    tb_trace_i("  %s -f sample.wav", program);
}

tb_int_t main(tb_int_t argc, tb_char_t** argv)
{
    // Initialize TBOX
    if (!tb_init(tb_null, tb_null)) {
        tb_trace_e("Failed to initialize TBOX!");
        return -1;
    }

    tb_bool_t verbose = tb_false;
    tb_char_t const* audio_file = tb_null;

    // Parse command line arguments
    for (tb_int_t i = 1; i < argc; i++) {
        if (tb_strcmp(argv[i], "-h") == 0 || tb_strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        else if (tb_strcmp(argv[i], "-v") == 0 || tb_strcmp(argv[i], "--verbose") == 0) {
            verbose = tb_true;
        }
        else if ((tb_strcmp(argv[i], "-f") == 0 || tb_strcmp(argv[i], "--file") == 0) && i + 1 < argc) {
            audio_file = argv[++i];
        }
        else {
            tb_trace_e("Unknown option: %s", argv[i]);
            print_usage(argv[0]);
            return -1;
        }
    }

    tb_trace_i("TBOX Audio Application Started");
    
    if (verbose) {
        tb_trace_i("Verbose mode enabled");
    }

    if (audio_file) {
        tb_trace_i("Processing audio file: %s", audio_file);
        
        // Example of using TBOX stream to read an audio file
        tb_stream_ref_t stream = tb_stream_init_from_url(audio_file);
        if (stream) {
            if (tb_stream_open(stream)) {
                tb_hize_t file_size = tb_stream_size(stream);
                tb_trace_i("Audio file size: %llu bytes", file_size);

                if (verbose) {
                    tb_byte_t header[128];
                    tb_long_t read = tb_stream_read(stream, header, sizeof(header));
                    if (read > 0) {
                        tb_trace_i("First %ld bytes of file:", read);
                        tb_dump_data(header, read);
                    }
                }
                
                tb_stream_clos(stream);
            } else {
                tb_trace_e("Could not open audio file: %s", audio_file);
            }
            tb_stream_exit(stream);
        } else {
            tb_trace_e("Could not create stream for: %s", audio_file);
        }
    } else {
        tb_trace_i("No audio file specified. This is a demonstration of audio processing concepts.");
        tb_trace_i("");
        tb_trace_i("Supported operations:");
        tb_trace_i("- File I/O using TBOX streams");
        tb_trace_i("- Memory management for audio buffers");
        tb_trace_i("- Platform abstraction for audio APIs");
        tb_trace_i("- Data processing utilities");
    }

    tb_trace_i("TBOX Audio Application Finished");
    
    // Exit TBOX
    tb_exit();
    return 0;
}