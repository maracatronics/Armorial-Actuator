/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include <QCoreApplication>
#include <exithandler.h>
#include <packetmanager.h>

/* backbone (localhost?) defines */
#define BACKBONE_ADDRESS "127.0.0.1"
#define BACKBONE_PORT 0

/* grsim host defines */
#define GRSIM_ADDRESS "127.0.0.1"
#define GRSIM_PORT 20011

/* thread defines */
#define LOOP_FREQUENCY 60 // hz

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ExitHandler::setup(&a);

    // Command line parser, get arguments
    QCommandLineParser parser;
    parser.setApplicationDescription("Actuator application help.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("grSimAddress", "Sets the grSim command address (default: '127.0.0.1').");
    parser.addPositionalArgument("grSimPort", "Sets the grSim command address (default: '20011').");
    parser.process(a);
    QStringList args = parser.positionalArguments();

    /// Args
    QString grSimAddress = "127.0.0.1";
    quint16 grSimPort = 20011;

    /// Check arguments
    // Team color
    if(args.size() >= 1) {
        grSimAddress = args.at(0);
    }
    // Field side
    if(args.size() >= 2) {
        grSimPort = static_cast<quint16>(args.at(1).toInt());
    }

    std::cout << "[ACTUATOR] Actuator running on address " << grSimAddress.toStdString() << " and port " << grSimPort << std::endl;

    PacketManager packetManager("Armorial-Actuator");
    packetManager.connect(BACKBONE_ADDRESS, BACKBONE_PORT, grSimAddress, grSimPort);
    packetManager.setLoopFrequency(LOOP_FREQUENCY);
    packetManager.start();

    // wait
    int retn = a.exec();

    // finish
    packetManager.stopRunning();
    packetManager.wait();

    return retn;
}
