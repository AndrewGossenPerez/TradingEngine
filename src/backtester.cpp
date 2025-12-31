
#include <iostream>
#include "core/types.hpp"
#include "data/bar.hpp"
#include "backtesting/backtesting.hpp"
#include "backtesting/strategies.hpp"
#include "events/dispatcher.hpp"
#include "events/events.hpp"
#include "data/market_state.hpp"
#include "pipeline/strategy_handler.hpp"
#include "pipeline/risk_handler.hpp"
#include "pipeline/portfolio_handler.hpp"
#include "pipeline/excecution_handler.hpp"
#include "pipeline/trace_handler.hpp"
#include "pipeline/report_handler.hpp"

trd::Result trd::Backtest::run(const std::vector<trd::Bar>& bars,Strategy& strategy){

    std::cout << "Backtest begun ... " << std::endl;

    // Event-driven backtester 

    trd::Result result;

    if (bars.size()<2) { // Not enough sufficent bars 
        result.finalEquity= bars.empty() ? 0.0 : m_portfolio.equity(bars.back().close);
        return result;
    }

    events::Dispatcher bus;
    MarketState marketState;

    // Create the main event-pipleine classes
    StrategyHandler handlerStrat(strategy,bus);
    RiskHandler handlerRisk(m_portfolio,bus);
    ExcecutionHandler handlerExce(m_exce,marketState,bus);
    PortfolioHandler handlerPort(m_portfolio);
    //TraceHandler handlerTrace;
    ReportHandler handlerReport(marketState,m_portfolio,result);

    // Subscribe each class, i.e bind them to events using lambdas
    bus.subscribe([&](const events::Event& event){handlerStrat.on(event);});
    bus.subscribe([&](const events::Event& event){handlerRisk.on(event);});
    bus.subscribe([&](const events::Event& event){handlerExce.on(event);});
    bus.subscribe([&](const events::Event& event){handlerPort.on(event);});
    //bus.subscribe([&](const events::Event& event){handlerTrace.on(event);});
    bus.subscribe([&](const events::Event& event){handlerReport.on(event);});

    // Main loop 
    for (std::size_t i=0; i+1 < bars.size();++i){
        marketState.current=bars[i];
        marketState.next=bars[i+1];
        // Create the market event 
        bus.push(
            events::MarketEvent{marketState.current,marketState.next}
        );
        bus.dispatchAll();
    }

    result.finalEquity=m_portfolio.equity(bars.back().close);
    result.trades=handlerReport.getTrades();
    result.equityPoints=handlerReport.getEquityPoints();

    return result;

}