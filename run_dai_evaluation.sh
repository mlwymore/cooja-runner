#!/bin/bash

#REMOTE_DIRECTORY="dai20160928"
AUX_DIRECTORY="/home/user/cooja-runner"
CONTIKI_DIRECTORY="/home/user/contiki"
COOJA_JAR_PATH="$CONTIKI_DIRECTORY/tools/cooja/dist/cooja.jar"
RESULTS_DIRECTORY="/home/user/cooja-results"
SOURCE_DIRECTORY="$CONTIKI_DIRECTORY/examples/mysourceappdir"
SINK_DIRECTORY="$CONTIKI_DIRECTORY/examples/mysinkappdir"
APP_FILE_PATH="/home/user/cooja-runner/example-broadcast.c"
CSC_LIST=("/home/user/cooja-runner/template_udgm_cooja_config.csc")
PROJ_CONF_LIST=("$AUX_DIRECTORY/template_project_conf.h")

if [ ! -d "$RESULTS_DIRECTORY" ]; then
	mkdir -p $RESULTS_DIRECTORY
fi

rm $RESULTS_DIRECTORY/*.txt

NUMRUNS=20

NUMPACKETS=250

CHANNEL_CHECK_RATE=8
for PROJ_CONF in "${PROJ_CONF_LIST[@]}"; do
	sed -i 's/NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE.*/NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE/' $PROJ_CONF
done

cd $SINK_DIRECTORY
make TARGET=sky

sed -i 's/\#define NUM_PACKETS_TO_SEND.*/\#define NUM_PACKETS_TO_SEND '$NUMPACKETS'/' $APP_FILE_PATH

DAIS=(1 5 25 50 75 100 125 150)
for DAI in "${DAIS[@]}"; do
	sed -i 's/\#define DATA_ARRIVAL_INTERVAL.*/\#define DATA_ARRIVAL_INTERVAL '$DAI'/' $APP_FILE_PATH
	TIMEOUT_MS=$((NUMPACKETS * (DAI + 1) * 1000 * 2))
        for CSC in "{CSC_LIST[@]}"; do
		sed -i 's/"-dai.*/"-dai'$DAI'-" +/' $CSC
		sed -i 's/^TIMEOUT.*/TIMEOUT('$TIMEOUT_MS');/' $CSC
	done

	cd $SOURCE_DIRECTORY
	make TARGET=sky

	for ((i=0;i<NUMRUNS;i++)); do
		SEED=$RANDOM
		
		for CSC in "{CSC_LIST[@]}"; do
			java -mx1024m -jar $COOJA_JAR_PATH -nogui=$CSC -contiki=$CONTIKI_DIRECTORY -random-seed=$SEED
		done
	done
done

#find $RESULTS_DIRECTORY -type f -exec curl -1 -v --disable-epsv --ftp-skip-pasv-ip -u user@place.net:pass\ word -ftp-ssl -T {} ftp://ftp.box.com/path/to/$REMOTE_DIRECTORY/ --ftp-create-dirs \;

exit 0
