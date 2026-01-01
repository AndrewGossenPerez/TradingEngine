
#pragma once 
#include "events/events.hpp"
#include "core/types.hpp"
#include "data/market_state.hpp"
#include "core/portfolio.hpp"
#include "backtesting/backtesting.hpp"
#include <vector>


class ReportHandler{ 

    public:

    ReportHandler(trd::MarketState& marketState,Portfolio& portfolio,trd::Result& result) : m_marketState(marketState), 
    m_portfolio(portfolio), m_result(result) {}

    void on(const events::Event& event){

        if (auto* ev=std::get_if<events::FillEvent>(&event)){
            // Log this trade 
            m_trades.emplace_back( 
                    ev->epoch,
                    ev->side,
                    ev->qty,
                    ev->px,
                    ev->fee
            );

            m_result.netFees+=ev->fee;

        } else if (auto* ev=std::get_if<events::MarketEvent>(&event)){
            // Log equity 
            m_equityCurve.emplace_back( 
                    m_marketState.current.epoch,
                    m_portfolio.equity(m_marketState.current.close)
            );

        }

    }

    std::vector<trd::TradeLog> getTrades() { return m_trades; }
    std::vector<trd::EquityPoint> getEquityPoints() { return m_equityCurve; }

    private:

    trd::MarketState m_marketState;
    Portfolio m_portfolio;
    std::vector<trd::TradeLog> m_trades;
    std::vector<trd::EquityPoint> m_equityCurve;
    trd::Result& m_result;

};