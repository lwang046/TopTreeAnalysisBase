
#include "../interface/Trigger.h"

Trigger::Trigger(bool isMuon, bool isElectron, bool trigSingleLep, bool trigDoubleLep, bool trigTrilep, bool trigMET, bool trigJET):
triggerList(), triggermap(), muon(false), electron(false), singleLep(false), doubleLep(false), trilep(false), fullHadr(false), met(false), jetht(false),trigged(false), redotrigmap(false), currentRunTrig(0), previousRunTrig(-1), currentFilenameTrig(""), previousFilenameTrig(""), iFileTrig(-1), treeNumberTrig(-1)
{
  if(trigJET){
    jetht = true;
  }
  if(trigMET)
  {
     met = true; 
  }
  if (isMuon)
  {
    muon = true;
  }
  if (isElectron)
  {
    electron = true;
  }
  else if(!isMuon && !isElectron)
  {
    cout << "TRIGGER::TRIGGER - No selected lepton..." << endl;
  }
  if (trigSingleLep)
  {
    singleLep = true;
  }
  if (trigDoubleLep)
  {
    doubleLep = true;
  }
  if(trigTrilep){
    trilep = true;
  }
  if (! trigSingleLep && ! trigDoubleLep && !trigTrilep)
  {
    fullHadr = true;
    cout << "TRIGGER::TRIGGER - Do you really want no lepton triggers?" << endl;
  }
}

Trigger::~Trigger()
{
  
}

void Trigger::bookTriggers(bool isData, string dName)
{
  /// This function is called in the dataset loop
  //  Reset all quantities here
  triggerList.clear();
  currentRunTrig = 0;
  previousRunTrig = -1;
  currentFilenameTrig = previousFilenameTrig = "";
  iFileTrig = -1;
  treeNumberTrig = -1;
  triggermap.clear();
  
  /// Add relevant triggers to triggerlist
  //  Recommended triggers for TOP analyses
  //  Last updated: 10 May 2017 . https://twiki.cern.ch/twiki/bin/viewauth/CMS/TopTrigger
  
  if(met){
   
    triggerList.push_back("HLT_PFHT300_PFMET110_v*");
    triggerList.push_back("HLT_PFMET200_v*");
    triggerList.push_back("HLT_PFMET300_v*");
    triggerList.push_back("HLT_PFMET120_PFMHT120_IDTight_v*");
    triggerList.push_back("HLT_PFMET170_HBHECleaned_v*");
    /*triggerList.push_back("HLT_Ele15_IsoVVVL_PFHT350_PFMET50_v*");
    triggerList.push_back("HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_PFMET40_v*");
    triggerList.push_back("HLT_PFMET120_BTagCSV_p067_v*");
    triggerList.push_back("HLT_PFMET120_Mu5_v*");
    triggerList.push_back("HLT_Mu6_PFHT200_PFMET80_BTagCSV_p067_v*");
    triggerList.push_back("HLT_Mu15_IsoVVVL_BTagCSV_p067_PFHT400_v*");
    triggerList.push_back("HLT_Mu15_IsoVVVL_PFHT350_PFMET50_v*");
   */
  }
  else if(jetht){
    triggerList.push_back("HLT_QuadJet45_TripleBTagCSV_p087_v*");
    triggerList.push_back("HLT_QuadJet45_DoubleBTagCSV_p087_v*");
    triggerList.push_back("HLT_DoubleJet90_Double30_TripleBTagCSV_p087_v*");
    triggerList.push_back("HLT_DoubleJet90_Double30_DoubleBTagCSV_p087_v*");
    triggerList.push_back("HLT_PFHT400_SixJet30_DoubleBTagCSV_p056_v*");
    triggerList.push_back("HLT_HT200_v*");
    triggerList.push_back("HLT_HT275_v*");
    triggerList.push_back("HLT_HT325_v*");
  }
  else if(singleLep && !doubleLep && !trilep){
    if (muon)
    {
          triggerList.push_back("HLT_IsoMu24_v*");
          triggerList.push_back("HLT_IsoTkMu24_v*");
       /*   triggerList.push_back("HLT_IsoMu22_v*");
          triggerList.push_back("HLT_IsoTkMu22_v*");
          triggerList.push_back("HLT_IsoMu22_eta2p1_v*");
          triggerList.push_back("HLT_IsoTkMu22_eta2p1_v*");
       */
         }
    if (electron)
    {
          //triggerList.push_back("HLT_Ele32_eta2p1_WPTight_Gsf_v*");
          triggerList.push_back("HLT_Ele27_WPTight_Gsf_v*"); // relative unprescaled
          /*triggerList.push_back("HLT_Ele25_eta2p1_WPTight_Gsf_v*"); // relative unprescaled
          triggerList.push_back("HLT_Ele27_eta2p1_WPLoose_Gsf_v*");*/
    }
  }
  else if(doubleLep && !singleLep && !trilep){
    //cout << "muon " << muon << " electron " << electron << endl;
    if (muon && !electron)
    {
      if(dName.find("Data_Run2016H")==std::string::npos) triggerList.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v*"); // prescaled for H
      if(dName.find("Data_Run2016H")==std::string::npos) triggerList.push_back("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v*"); // prescaked for H
      if(dName.find("Data_Run2016H")!=std::string::npos) triggerList.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v*");
      if(dName.find("Data_Run2016H")!=std::string::npos) triggerList.push_back("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v*");
      
    }
    else if (electron && !muon)
    {
      triggerList.push_back("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*");
      // triggerList.push_back("HLT_DoubleEle24_22_eta2p1_WPLoose_Gsf_v*");
    }
    
    else if (muon && electron)
    {
      //triggerList.push_back("HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v*");
      //triggerList.push_back("HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ_v*");
      if(dName.find("Data_Run2016H")==std::string::npos) triggerList.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v*");
      if(dName.find("Data_Run2016H")==std::string::npos) triggerList.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v*");
      if(dName.find("Data_Run2016H")!=std::string::npos) triggerList.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*");
      if(dName.find("Data_Run2016H")!=std::string::npos) triggerList.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v*");
    }
    
  }
  else if(trilep && ! doubleLep && !singleLep){
    if(muon && !electron)
    {
      
      triggerList.push_back("HLT_TripleMu_12_10_5_v*");
      // triggerList.push_back("HLT_TripleMu_5_3_3_v*");
      
    }
    
    
    else if(!muon && electron)
    {
      triggerList.push_back("HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL_v*");
      
    }
    else if(muon && electron)
    {
      triggerList.push_back("HLT_DiMu9_Ele9_CaloIdL_TrackIdL_v*");
      triggerList.push_back("HLT_Mu8_DiEle12_CaloIdL_TrackIdL_v*");
      
    }
    
  }
  
  for(UInt_t iTrig = 0; iTrig < triggerList.size(); iTrig++)
  {
    triggermap[triggerList[iTrig]] = std::pair<int,bool> (-999,false);
  }
  
}

void Trigger::checkAvail(int currentRunTrig, vector < Dataset* > datasets, unsigned int d, TTreeLoader *treeLoader, TRootEvent* event, bool verbose)
{
  redotrigmap = false;
  treeNumberTrig = datasets[d]->eventTree()->GetTreeNumber();
  currentFilenameTrig = datasets[d]->eventTree()->GetFile()->GetName();
  if (previousFilenameTrig != currentFilenameTrig)
  {
    previousFilenameTrig = currentFilenameTrig;
    iFileTrig++;
    redotrigmap = true;
  }
  if (previousRunTrig != currentRunTrig)
  {
    previousRunTrig = currentRunTrig;
    cout << "What Run: " << previousRunTrig << endl;
    redotrigmap=true;
  }
  
  if (verbose && redotrigmap)
  {
    treeLoader->ListTriggers(currentRunTrig, treeNumberTrig);
  }
  
  
  // get trigger info:
  for(std::map<std::string,std::pair<int,bool> >::iterator iter = triggermap.begin(); iter != triggermap.end(); iter++)
  {
    if (redotrigmap)
    {
      Int_t loc = treeLoader->iTrigger(iter->first, currentRunTrig, treeNumberTrig);
      string trigname = iter->first;
      //cout << "trigname: " << trigname << "  location: " << loc << /*"  " << currentFilenameTrig << "  " << currentRunTrig <<*/ endl;
      
      iter->second.first = loc;
    }
    // and check if it exists and if it fired:
    if (iter->second.first >= 0 && iter->second.first != 9999) // trigger exists
      iter->second.second = event->trigHLT(iter->second.first);
    else
      iter->second.second = false;
  }
}


int Trigger::checkIfFired()
{
  // now check if the appropriate triggers fired for each analysis:
  trigged = 0;
  
  for(UInt_t itrig = 0; itrig < triggerList.size() && trigged == 0; itrig++)
  {
    if (triggermap[triggerList[itrig]].second)
      trigged = 1;
  }
  
  return trigged;
}
