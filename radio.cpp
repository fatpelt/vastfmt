#include "propertydefs.h"
#include "commanddefs.h"
#include "logging.h"
#include "fmtx.h"
#include "commands.h"

#include <iostream>
#include <vector>
#include <functional> // reference_wrapper

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

// Helper that won't allow me to add a second reference to the list.  This is
// useful because I use singletons (for other design reasons) and am avoiding
// multiple instances of configured types.
#define add_to(v, e)					\
	{									\
										\
	bool found = false;					\
	for ( VastCommand& c : commands )	\
		if ( &c == &e )					\
			found = true;				\
	if ( ! found )						\
		v.push_back(e);					\
										\
	}

int impedance = -1;

uint16_t parseInt(const char *PI)
{
	return (uint16_t)strtol(PI, NULL, 16);

	uint16_t pi_val = 0;

	sscanf(PI, "%x%x", &pi_val, (&pi_val+1));

	return pi_val;
}

// Based on code found at:
// http://www.w9wi.com/articles/rdsreverse.htm
uint16_t call_to_pi(char *call)
{
	uint16_t picode = 0;
	int i = 0;

	// Convert string to upper case
	for ( i = 0; i < strlen(call); ++i )
		call[i] = toupper(call[i]);

	if ( strlen(call) == 4 )
	{
		int letter1 = call[0]-65;
		int letter2 = call[1]-65;
		int letter3 = call[2]-65;
		int letter4 = call[3]-65;

		picode = (26 * 26 * letter2) + (26 * letter3) + letter4;
		if ( letter1 == 10 ) {
			picode = picode + 4096;
		} else {
			picode = picode + 21672;
		}
	}
	else if ( strlen(call) == 3 )
	{
		if ( strcmp("KBW", call) == 0 )
			picode = parseInt("0x99A5");
		else if ( strcmp("KCY", call) == 0 )
			picode = parseInt("0x99A6");
		else if ( strcmp("KDB", call) == 0 )
			picode = parseInt("0x9990");
		else if ( strcmp("KDF", call) == 0 )
			picode = parseInt("0x99A7");
		else if ( strcmp("KEX", call) == 0 )
			picode = parseInt("0x9950");
		else if ( strcmp("KFH", call) == 0 )
			picode = parseInt("0x9951");
		else if ( strcmp("KFI", call) == 0 )
			picode = parseInt("0x9952");
		else if ( strcmp("KGA", call) == 0 )
			picode = parseInt("0x9953");
		else if ( strcmp("KGB", call) == 0 )
			picode = parseInt("0x9991");
		else if ( strcmp("KGO", call) == 0 )
			picode = parseInt("0x9954");
		else if ( strcmp("KGU", call) == 0 )
			picode = parseInt("0x9955");
		else if ( strcmp("KGW", call) == 0 )
			picode = parseInt("0x9956");
		else if ( strcmp("KGY", call) == 0 )
			picode = parseInt("0x9957");
		else if ( strcmp("KHQ", call) == 0 )
			picode = parseInt("0x99AA");
		else if ( strcmp("KID", call) == 0 )
			picode = parseInt("0x9958");
		else if ( strcmp("KIT", call) == 0 )
			picode = parseInt("0x9959");
		else if ( strcmp("KJR", call) == 0 )
			picode = parseInt("0x995A");
		else if ( strcmp("KLO", call) == 0 )
			picode = parseInt("0x995B");
		else if ( strcmp("KLZ", call) == 0 )
			picode = parseInt("0x995C");
		else if ( strcmp("KMA", call) == 0 )
			picode = parseInt("0x995D");
		else if ( strcmp("KMJ", call) == 0 )
			picode = parseInt("0x995E");
		else if ( strcmp("KNX", call) == 0 )
			picode = parseInt("0x995F");
		else if ( strcmp("KOA", call) == 0 )
			picode = parseInt("0x9960");
		else if ( strcmp("KOB", call) == 0 )
			picode = parseInt("0x99AB");
		else if ( strcmp("KOY", call) == 0 )
			picode = parseInt("0x9992");
		else if ( strcmp("KPQ", call) == 0 )
			picode = parseInt("0x9993");
		else if ( strcmp("KQV", call) == 0 )
			picode = parseInt("0x9964");
		else if ( strcmp("KSD", call) == 0 )
			picode = parseInt("0x9994");
		else if ( strcmp("KSL", call) == 0 )
			picode = parseInt("0x9965");
		else if ( strcmp("KUJ", call) == 0 )
			picode = parseInt("0x9966");
		else if ( strcmp("KUT", call) == 0 )
			picode = parseInt("0x9995");
		else if ( strcmp("KVI", call) == 0 )
			picode = parseInt("0x9967");
		else if ( strcmp("KWG", call) == 0 )
			picode = parseInt("0x9968");
		else if ( strcmp("KXL", call) == 0 )
			picode = parseInt("0x9996");
		else if ( strcmp("KXO", call) == 0 )
			picode = parseInt("0x9997");
		else if ( strcmp("KYW", call) == 0 )
			picode = parseInt("0x996B");
		else if ( strcmp("WBT", call) == 0 )
			picode = parseInt("0x9999");
		else if ( strcmp("WBZ", call) == 0 )
			picode = parseInt("0x996D");
		else if ( strcmp("WDZ", call) == 0 )
			picode = parseInt("0x996E");
		else if ( strcmp("WEW", call) == 0 )
			picode = parseInt("0x996F");
		else if ( strcmp("WGH", call) == 0 )
			picode = parseInt("0x999A");
		else if ( strcmp("WGL", call) == 0 )
			picode = parseInt("0x9971");
		else if ( strcmp("WGN", call) == 0 )
			picode = parseInt("0x9972");
		else if ( strcmp("WGR", call) == 0 )
			picode = parseInt("0x9973");
		else if ( strcmp("WGY", call) == 0 )
			picode = parseInt("0x999B");
		else if ( strcmp("WHA", call) == 0 )
			picode = parseInt("0x9975");
		else if ( strcmp("WHB", call) == 0 )
			picode = parseInt("0x9976");
		else if ( strcmp("WHK", call) == 0 )
			picode = parseInt("0x9977");
		else if ( strcmp("WHO", call) == 0 )
			picode = parseInt("0x9978");
		else if ( strcmp("WHP", call) == 0 )
			picode = parseInt("0x999C");
		else if ( strcmp("WIL", call) == 0 )
			picode = parseInt("0x999D");
		else if ( strcmp("WIP", call) == 0 )
			picode = parseInt("0x997A");
		else if ( strcmp("WIS", call) == 0 )
			picode = parseInt("0x99B3");
		else if ( strcmp("WJR", call) == 0 )
			picode = parseInt("0x997B");
		else if ( strcmp("WJW", call) == 0 )
			picode = parseInt("0x99B4");
		else if ( strcmp("WJZ", call) == 0 )
			picode = parseInt("0x99B5");
		else if ( strcmp("WKY", call) == 0 )
			picode = parseInt("0x997C");
		else if ( strcmp("WLS", call) == 0 )
			picode = parseInt("0x997D");
		else if ( strcmp("WLW", call) == 0 )
			picode = parseInt("0x997E");
		else if ( strcmp("WMC", call) == 0 )
			picode = parseInt("0x999E");
		else if ( strcmp("WMT", call) == 0 )
			picode = parseInt("0x999F");
		else if ( strcmp("WOC", call) == 0 )
			picode = parseInt("0x9981");
		else if ( strcmp("WOI", call) == 0 )
			picode = parseInt("0x99A0");
		else if ( strcmp("WOL", call) == 0 )
			picode = parseInt("0x9983");
		else if ( strcmp("WOR", call) == 0 )
			picode = parseInt("0x9984");
		else if ( strcmp("WOW", call) == 0 )
			picode = parseInt("0x99A1");
		else if ( strcmp("WRC", call) == 0 )
			picode = parseInt("0x99B9");
		else if ( strcmp("WRR", call) == 0 )
			picode = parseInt("0x99A2");
		else if ( strcmp("WSB", call) == 0 )
			picode = parseInt("0x99A3");
		else if ( strcmp("WSM", call) == 0 )
			picode = parseInt("0x99A4");
		else if ( strcmp("WWJ", call) == 0 )
			picode = parseInt("0x9988");
		else if ( strcmp("WWL", call) == 0 )
			picode = parseInt("0x9989");
	}

	return picode;
}

uint8_t watts_to_dbuv(double watts)
{
	if ( impedance < 0 )
	{
		logwrite(LOG_ERROR, "Must have '--impedance' argument before argument '--power-watts'");
		exit(EXIT_FAILURE);
	}

    double dBW = 10 * log10(watts);
    double dBm = dBW + 30;
    double p = pow(10, dBm / 10) / 1000;
    double u = sqrt(p * impedance);
    double dBuV = 20 * log10(u * 1000000);
    return dBuV;
}

void usage(char *appname)
{
printf("Usage: %s [OPTION...]\n"
"\n"
"%s is a command line application to control the VastElectonics V-FMT212R\n"
"USB FM transmitter.\n"
"\n"
"Note that currently nothing that writes to EEPROM is supported.  You will likely\n"
"want to configure your transmitter using the Windows app and simply use t his to\n"
"tweak things on the fly, but support for that should be coming soon.\n"
"\n"
"Options:\n"
"\t-t, --transmit\t\tEnable transmitter\n\n"
"\t-n, --no-transmit\tDisable transmitter\n\n"
"\t-f, --tune-frequency\tSet the transmitter's frequency in MHz\n\n"
"\t-s, --rds-station\tSet the call sign of your station (PS)\n\n"
"\t-R, --rds-text\t\tSet the RDS text (RT)\n\n"
// http://www.pira.cz/rds/show.asp?art=rds_encoder_support
//  -> http://www.pira.cz/rds/rtpclass.pdf
"\t-T, --title\t\tSet the title of the track (RT+ code 1)\n\n"
"\t-A, --artist\t\tSet the artist of the track (RT+ code 4)\n\n"
"\t-p, --power\t\tSet the transmit power in dBµV\n\n"
"\t-a, --antenna-cap\tSet the antenna capacitance in pF\n\n"
"\t    --audio-deviation\tSet the audio deviation in Hz\n\n"
"\t-i, --impedance\t\tImpedance of your antenna cable in ohms.  Used in calculation\n"
"\t\t\t\tfor '--power-watts' and must be specified before '--power-watts'\n\n"
"\t    --power-watts\tSet the power in watts unit instead of dBµV.\n"
"\t\t\t\tNOTE: This requires that you specify the impedance of your\n"
"\t\t\t\tcable first with the '--impedance' argument\n\n"
"\t-e, --preemphasisid\tSet the Pre-Emphasis to 50µs (Europe) or 75µs (USA) by\n"
"\t\t\t\tspecifying the string \"USA\" or \"Europe\"\n\n"
"\t-P, --pilot\t\tEnable pilot tone encoding\n\n"
"\t    --no-pilot\t\tDisable pilot tone encoding\n\n"
"\t-D, --pilot-deviation\tSet the pilot deviation in Hz\n\n"
"\t-F, --pilot-frequency\tSet the pilot frequency in Hz\n\n"
"\t-S, --stereo\t\tEnable stereo transmission\n\n"
"\t-M, --mono\t\tDisable stereo transmission\n\n"
"\t    --rds\t\tEnable RDS transmission\n\n"
"\t    --no-rds\t\tDisable RDS transmission\n\n"
"\t-d, --rds-deviation\tSet the RDS deviation\n\n"
"\t    --reset\t\tAttempt to reset the frontend\n\n"
"\t-v, --verbose\t\tTurn up logging level.  Can be specified more than once for more logging\n\n"
"\t-h, --help\t\tDisplay this help\n"
,
	appname,
	appname);
}


int main(int argc, char *argv[])
{

	FMTX_MODE_ENUM ret = FMTX_MODE_OK;
	fmtxCmdSpecial = FMTX_SPECIAL_FRONTEND; // We want to work with frontend to set frequency
	loglevel = LOG_ERROR; // Turn down our logging
	char cpuid[32];
	char rev[32];
	int c;

	cpuid[0] = '\0';
	rev[0] = '\0';

	std::vector<std::reference_wrapper<VastCommand>> commands;

	while (1)
	{
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] =
		{
			{"transmit",			no_argument,		0, 't'},
			{"no-transmit",			no_argument,		0, 'n'},
			{"tune-frequency",		required_argument,	0, 'f'},
			{"rds-station",			required_argument,	0, 's'},
			{"rds-pi",			required_argument,	0, 'I'},
			{"rds-text",			required_argument,	0, 'R'},
			{"title",				required_argument,	0, 'T'},
			{"artist",				required_argument,	0, 'A'},
			{"power",				required_argument,	0, 'p'},
			{"impedance",			required_argument,	0, 'i'},
			{"power-watts",			required_argument,	0,  4 },
			{"antenna-cap",			required_argument,	0, 'a'},
			{"audio-deviation",		required_argument,	0,  6 },
			{"preemphasisid",		required_argument,	0, 'e'},
			{"pilot",				no_argument,		0, 'P'},
			{"no-pilot",			no_argument,		0,  1 },
			{"pilot-deviation",		required_argument,	0, 'D'},
			{"pilot-frequency",		required_argument,	0, 'F'},
			{"stereo",				no_argument,		0, 'S'},
			{"mono",				no_argument,		0, 'M'},
			{"rds",					no_argument,		0,  3 },
			{"no-rds",				no_argument,		0,  2 },
			{"rds-deviation",		required_argument,	0, 'd'},
			{"reset",				no_argument,		0,  5 },
			{"verbose",				no_argument,		0, 'v'},
			{"help",				no_argument,		0, 'h'},
			{"rds-repeat",			required_argument,	0,	7},
			{"rds-mix",			required_argument,	0,	8},
			{0,						0,					0,  0 }
		};

		c = getopt_long(argc, argv, "tnf:s:R:T:A:p:i:a:e:PD:F:SMd:vhI:",
		long_options, &option_index);
		if (c == -1)
			break;

		switch (c)
		{
			case 't':
			{
				VastTransmission& xmit = VastTransmission::get();
				xmit = true;
				add_to(commands, xmit);

				logwrite(LOG_INFO, "Enabled transmission");
				break;
			}
			case 'n':
			{
				VastTransmission& xmit = VastTransmission::get();
				xmit = false;
				add_to(commands, xmit);

				logwrite(LOG_INFO, "Diabled transmission");
				break;
			}
			case 'f': // tune-frequency
			{
				VastFrequency& freq = VastFrequency::get();
				freq = strtod(optarg, NULL);
				add_to(commands, freq);

				logwrite(LOG_INFO, "Set tune frequency to %f", strtod(optarg, NULL));
				break;
			}
			case 7: /* repeat */
			{
				VastPsMessage& psm = VastPsMessage::get();
				psm.mRdsRepeat = atoi(optarg);
				break;
			}
			case 8: /* mix */
			{
				VastPsMessage& psm = VastPsMessage::get();
				psm.mRdsMix = atoi(optarg);
				break;
			}
			case 'I': // rds-ps
			{
				VastPsMessage& psm = VastPsMessage::get();
				std::string psmessage = optarg;
				psm = psmessage;
				add_to(commands, psm);

				logwrite(LOG_INFO, "Set Station to %s", optarg);
				break;
			}
			case 's': // rds-station - for the rds pi
			{
				VastRdsPI& rdspi = VastRdsPI::get();
				rdspi = call_to_pi(optarg);
				add_to(commands, rdspi);

				logwrite(LOG_INFO, "Set RDS PI to %ld", call_to_pi(optarg));
				break;
			}
			case 'R': // rds-text
			{
				VastRDSRtText& rds = VastRDSRtText::get();
				rds = optarg;
				add_to(commands, rds);

				logwrite(LOG_INFO, "Set RDS RT to %s", optarg);
				break;
			}
			case 'T': // title
			{
				VastRDSRtText& rds = VastRDSRtText::get();
				std::string title = optarg;
				rds.setTitle(title);
				add_to(commands, rds);

				logwrite(LOG_INFO, "Set RDS RT+ code 1 (ITEM.TITLE) to %s", optarg);
				break;
			}
			case 'A': // artist
			{
				VastRDSRtText& rds = VastRDSRtText::get();
				std::string artist = optarg;
				rds.setArtist(artist);
				add_to(commands, rds);

				logwrite(LOG_INFO, "Set RDS RT+ code 4 (ITEM.ARTIST) to %s", optarg);
				break;
			}
			case 'p': // power
			{
				if ( atoi(optarg) < 88 )
				{
					VastPowerAntennaCap& pwr = VastPowerAntennaCap::get();
					pwr.setPower(88);
					add_to(commands, pwr);

					logwrite(LOG_WARN, "Power set too low, upping to 88dBµV");
				}
				else if ( atoi(optarg) > 120 )
				{
					VastPowerAntennaCap& pwr = VastPowerAntennaCap::get();
					pwr.setPower(120);
					add_to(commands, pwr);

					logwrite(LOG_WARN, "Power set too high, lowering to 120dBµV");
				}
				else
				{
					VastPowerAntennaCap& pwr = VastPowerAntennaCap::get();
					pwr.setPower(atoi(optarg));
					add_to(commands, pwr);

					logwrite(LOG_INFO, "Set transmit power to %hhddBµV", atoi(optarg));
				}

				break;
			}
			case 'a': // antenna-cap
			{
				VastPowerAntennaCap& cap = VastPowerAntennaCap::get();
				cap.setAntennaCap(strtod(optarg, NULL));
				add_to(commands, cap);

				logwrite(LOG_INFO, "Set antenna capacitor to %f", strtod(optarg, NULL));
				break;
			}
			case 6:
			{
				VastAudioDeviation& aDev = VastAudioDeviation::get();
				aDev = atoi(optarg);
				add_to(commands, aDev);

				logwrite(LOG_INFO, "Set audio deviation to %d", atoi(optarg));
				break;
			}
			case 'e': // preemphasisid
			{
				if ( strcmp(optarg, "USA") == 0 )
				{
					VastPreemphasisId& pre = VastPreemphasisId::get();
					pre = 0;
					add_to(commands, pre);

					logwrite(LOG_INFO, "Setting Preemphasis ID to 0 for USA");
				}
				else if ( strcmp(optarg, "Europe") == 0 )
				{
					VastPreemphasisId& pre = VastPreemphasisId::get();
					pre = 1;
					add_to(commands, pre);

					logwrite(LOG_INFO, "Setting Preemphasis ID to 1 for Europe");
				}
				else
				{
					logwrite(LOG_ERROR, "Couldn't determine Preemphasis ID");
					usage(argv[0]);
					exit(EXIT_FAILURE);
				}
				break;
			}
			case 'P': // pilot
			{
				VastPilotLmrRds& pilot = VastPilotLmrRds::get();
				pilot.setPilot(true);
				add_to(commands, pilot);

				logwrite(LOG_INFO, "Enabling pilot tone");
				break;
			}
			case  1:  // no-pilot
			{
				VastPilotLmrRds& pilot = VastPilotLmrRds::get();
				pilot.setPilot(false);
				add_to(commands, pilot);

				logwrite(LOG_INFO, "Disabling pilot tone");
				break;
			}
			case 'D': // pilot-deviation
			{
				VastPilotDeviation& pdev = VastPilotDeviation::get();
				pdev = atoi(optarg);
				add_to(commands, pdev);

				logwrite(LOG_INFO, "Setting pilot deviation to %d Hz", atoi(optarg));
				break;
			}
			case 'F': // pilot-frequency
			{
				VastPilotFrequency& pfreq = VastPilotFrequency::get();
				pfreq = atoi(optarg);
				add_to(commands, pfreq);

				logwrite(LOG_INFO, "Setting pilot frequency to %hd Hz", atoi(optarg));
				break;
			}
			case 'S': // stereo
			{
				VastPilotLmrRds& lmr = VastPilotLmrRds::get();
				lmr.setStereo(true);
				add_to(commands, lmr);

				logwrite(LOG_INFO, "Enabling stereo (Disabling mono)");
				break;
			}
			case 'M': // mono
			{
				VastPilotLmrRds& lmr = VastPilotLmrRds::get();
				lmr.setStereo(false);
				add_to(commands, lmr);

				logwrite(LOG_INFO, "Enabling mono (Disabling stereo)");
				break;
			}
			case 'i': // impedance
			{
				impedance = atoi(optarg);

				logwrite(LOG_INFO, "Setting impedance of antenna/cable to %uohms", impedance);
				break;
			}
			case  4: // power-watts
			{
				uint8_t power = watts_to_dbuv(strtod(optarg, NULL));
				if ( power < 88 )
				{
					logwrite(LOG_ERROR, "Power set too low, upping to 88dBµV");
					power = 88;
				}
				if ( power > 120 )
				{
					logwrite(LOG_ERROR, "Power set too high, lowering to 120dBµV");
					power = 120;
				}

				VastPowerAntennaCap& pwr = VastPowerAntennaCap::get();
				pwr.setPower(power);
				add_to(commands, pwr);

				logwrite(LOG_INFO, "Set transmit power to %hhddBµV", power);
				break;
			}

			case  5: // reset
			{
				VastReset& reset = VastReset::get();
				add_to(commands, reset);

				logwrite(LOG_INFO, "Resetting FE");
				break;
			}
			case  3: // rds
			{
				VastPilotLmrRds& rds = VastPilotLmrRds::get();
				rds.setRds(true);
				add_to(commands, rds);

				logwrite(LOG_INFO, "Enabling RDS");
				break;
			}
			case  2:  // no-rds
			{
				VastPilotLmrRds& rds = VastPilotLmrRds::get();
				rds.setRds(false);
				add_to(commands, rds);

				logwrite(LOG_INFO, "Disabling RDS");
				break;
			}
			case 'd': // rds-deviation
			{
				VastRdsDeviation& rdev = VastRdsDeviation::get();
				rdev = atoi(optarg);
				add_to(commands, rdev);
				logwrite(LOG_INFO, "Set RDS deviation to %d", atoi(optarg));
				break;
			}
			case 'v': // verbose
			{
				logwrite(loglevel, "Upping verbosity");
				if ( loglevel < 5 )
					loglevel++;
				break;
			}
			case 'h': //help
			{
				usage(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			}
			default:
			{
				usage(argv[0]);
				exit(EXIT_FAILURE);
			}
		}
	}

	if ( commands.size() == 0 )
	{
		logwrite(LOG_INFO, "Nothing to do!");
		exit(EXIT_SUCCESS);
	}

	ret = (FMTX_MODE_ENUM) fmtxIoAppIdString(cpuid, rev);
	if (ret != FMTX_MODE_OK)
	{
		logwrite(LOG_ERROR,"Can't get id string!");
		exit(EXIT_FAILURE);
	}
	logwrite(LOG_INFO,"Got CPU ID: %s rev.: %s", cpuid, rev);

	for ( VastCommand& cmd : commands )
	{
		cmd.printParameters();
		cmd.saveParametersToVast();
	}

	return 0;
}
