////////////////////////////////////////////////////////////////////////////////
// PMT Class definition
//
// mailto:ascarpell@bnl.gov
////////////////////////////////////////////////////////////////////////////////

#ifndef  __PMT_H
#define __PMT_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include "Geometry.h"
#include "Waveform.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TFile.h"
#include "TDirectory.h"

using namespace std;

class PMT
{

  public:

    PMT();
    PMT(int run, int board, int channel );
    PMT(int run, int pmt_number );
    ~PMT();

    // Loaders
    void loadWaveform( Waveform *waveform );

    // Getters
    int getRun(){ return m_run; };
    int getBoard(){ return m_board; };
    int getChannel(){ return m_channel; }
    int getPmtNumber(){ return m_pmt_number; }
    double getHV(){return m_hv_value; }
    void getBoardAndChannel( int pmt_number, int & board, int & channel);

    void setHV(double value){m_hv_value = value; }

    // vector<double> getAmplitudes(){ return m_amplitude_array; };

    // Helpers
    void clean();
    void initHist();
    void writeHist(TFile* thefile, TDirectory* ampDir, TDirectory* chargeDir);
    bool isIlluminated( int optical_channel );
    bool isHVon(){ if(m_hv_value!=0) is_HVon=true; return is_HVon;}

  private:

    int m_run;
    int m_board;
    int m_channel;
    int m_pmt_number;

    double m_hv_value;
    bool is_HVon=false;

    // vector<double> m_amplitude_array;
    // variable distributions for a PMT pulses
    TH1D *h_amplitude; //amplitude of pulse
    TH1D *h_amplitude_low; // amplitude of pulse in a smaller range
    TH1D *h_charge; // charge of pulse
    TH1D* h_pulsetime; //start time of pulse 
    TH1D* h_pulsepeaktime; //peak time of pulse
    TH1I* h_NbOfPulse; // number of pulses in a waveform

};

#endif //__PMT_H
