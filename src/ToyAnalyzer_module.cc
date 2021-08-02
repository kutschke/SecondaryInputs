//
// Read the toy data products form the event.
//
// Original author Rob Kutschke
//

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

#include "canvas/Utilities/InputTag.h"

#include "fhiclcpp/types/Sequence.h"

#include <iostream>
#include <vector>

using namespace std;

namespace mu2e {

  class ToyAnalyzer : public art::EDAnalyzer {
  public:

    struct Config {
      fhicl::Sequence<art::InputTag> tags {
        fhicl::Name("tags"),
          fhicl::Comment("InputTags for the products to read."),
      };
    };

    using Parameters = art::EDAnalyzer::Table<Config>;
    explicit ToyAnalyzer(const Parameters& conf);

    void analyze( const art::Event& e) override;

  private:

    std::vector<art::InputTag> _tags;

  };

  ToyAnalyzer::ToyAnalyzer(const Parameters& conf):
    art::EDAnalyzer{conf},
    _tags(conf().tags())
  {
  }

  void ToyAnalyzer::analyze(const art::Event& event) {

    std::cout << "Event: " << event.id() << std::endl;
    for ( auto tag : _tags ){
      if ( auto h = event.getHandle<int>(tag) ){
        std::cout <<  "   " << tag << ":  " << *h << std::endl;
      }
    }

  } // end ToyAnalyzer::analyze

}  // end namespace mu2e

DEFINE_ART_MODULE(mu2e::ToyAnalyzer)
