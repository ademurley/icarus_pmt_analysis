////////////////////////////////////////////////////////////////////////////////
// PMT Methods implementation
//
// mailto:ascarpell@bnl.gov
////////////////////////////////////////////////////////////////////////////////
#include "Utils.h"
#include "Pmt.h"

PMT::PMT( )
{ };

//------------------------------------------------------------------------------

PMT::PMT( int run, int board, int channel )
  : m_run(run)
  , m_board(board)
  , m_channel(channel)
{
  m_pmt_number=m_channel+geo::nchannels*m_board;
  this->initHist();
};

//------------------------------------------------------------------------------

PMT::PMT( int run, int pmt_number)
  : m_run(run)
  , m_pmt_number(pmt_number)
{
  m_board=0; m_channel=0;
  this->getBoardAndChannel(m_pmt_number, m_board, m_channel);
  this->initHist();
};

//------------------------------------------------------------------------------

PMT::~PMT()
{};

//------------------------------------------------------------------------------

void PMT::initHist()
{
  char hname[100];

  // Amplitude histograms
  sprintf(hname, "hist_board%d_channel%d_amplitude", m_board,  m_channel);
  h_amplitude = new TH1D(hname, hname, 100, 0, 2000);
  h_amplitude->SetXTitle("Amplitude (ADC counts)");
  h_amplitude->SetYTitle("Counts");

  sprintf(hname, "hist_board%d_channel%d_amplitude_low", m_board,  m_channel);
  h_amplitude_low = new TH1D(hname, hname, 100, 0, 150);
  h_amplitude_low->SetXTitle("Amplitude (ADC counts)");
  h_amplitude_low->SetYTitle("Counts");

  // for charge histograms
  sprintf(hname, "hQ_board%d_ch%d" , m_board, m_channel);
  h_charge = new TH1D(hname, hname, 200,0,100);
  h_charge->SetXTitle("Charge (pC)");
  h_charge->SetYTitle("Counts");
  // for pulse time
  sprintf(hname, "hPulseTime_board%d_ch%d", m_board, m_channel);
  h_pulsetime = new TH1D(hname, hname, 500,400,500);
  h_pulsetime->SetXTitle("Pulse start time (ns)");
  h_pulsetime->SetYTitle("Counts");
  sprintf(hname, "hPulsePeakTime_board%d_ch%d", m_board, m_channel);
  h_pulsepeaktime = new TH1D(hname, hname, 500,400,500);
  h_pulsepeaktime->SetXTitle("Pulse start time (ns)");
  h_pulsepeaktime->SetYTitle("Counts");
  // for number of pulse couting
  sprintf(hname, "hPulseCounting_board%d_ch%d", m_board, m_channel);
  h_NbOfPulse = new TH1I(hname, hname, 15,0,15);
  h_NbOfPulse->SetXTitle("Number of pulses in the time set window");
  h_NbOfPulse->SetYTitle("Counts");
  
};

//------------------------------------------------------------------------------

void PMT::getBoardAndChannel( int pmt_number, int & board, int & channel )
{
  //Return the board and channel number given the absolute pmt number
  board = pmt_number/geo::nchannels;
  channel = pmt_number%geo::nchannels;
};

//------------------------------------------------------------------------------

bool PMT::isIlluminated( int optical_channel )
{
  // True if a pmt is illuminated by a given optical channel with direct light
  bool m_isIlluminated=false;

  int laser_groups=10;

  // We compare the optical channel with the absolute pmt number.
  // NB: This assumption works only in the case all boards are taken and optical
  // channel number is changed in progressive increasing order.
  if( m_pmt_number>=optical_channel*laser_groups &
                                m_pmt_number<(optical_channel+1)*laser_groups ){
      m_isIlluminated=true;
  }

  return m_isIlluminated;
};

//------------------------------------------------------------------------------

void PMT::loadWaveform( Waveform *waveform )
{

  // Fetch the relevant quantities from the waveform
  double amplitude = abs( waveform->getAmplitude() );
  double charge = abs( waveform->getCharge() );
  double pulsestarttime = waveform->getStartTime();
  double pulsepeaktime = waveform->getPeakTime();
  int npulse = waveform->getNPulses();

  // Fill the histograms
  h_amplitude->Fill(amplitude);
  h_amplitude_low->Fill(amplitude);

  h_charge->Fill(charge);
  h_pulsetime->Fill(pulsestarttime);
  h_pulsepeaktime->Fill(pulsepeaktime);
  h_NbOfPulse->Fill(npulse);

  // Fill the arrays
  // m_amplitude_array.push_back(amplitude);

};

//------------------------------------------------------------------------------

void PMT::writeHist(TFile* thefile, TDirectory* ampDir, TDirectory* chargeDir)
{
  
  // Write the histograms to file
  ampDir->cd();
  h_amplitude->Write();
  h_amplitude_low->Write();

  chargeDir->cd();
  h_charge->Write();

};

//------------------------------------------------------------------------------

void PMT::clean()
{
  // m_amplitude_array.clear();
};

//------------------------------------------------------------------------------
