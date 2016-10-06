/* -*- c++ -*- */
/* 
 * Copyright 2015 Epiq Solutions.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef SIDEKIQ_TX_H
#define SIDEKIQ_TX_H

#include <sidekiq/sidekiq_defs.h>
#include <boost/scoped_ptr.hpp>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdint.h>
#include <string>
#include <netdb.h>
#include <string>
#include <map>

#include "srfs_interface.h"

namespace gr {
    namespace sidekiq {

	/*! String representation of STATUS enum */
	const std::string status_str[STATUS_INVALID] = {
	    "enabled",
	    "disabled"
	};

	    /*!< state of sidekiq connection */
	    enum state {
		STATE_STOPPED, STATE_STARTED,
	    };

    class sidekiq_tx {
    
    public:

        sidekiq_tx(const char *ip_addr, unsigned short port);
        ~sidekiq_tx();

	void start();
	void stop();


        uint64_t set_center_freq(uint64_t freq);
        uint64_t center_freq(void);

        uint32_t set_sample_rate(uint32_t sample_rate);
        uint32_t sample_rate(void);

        uint32_t set_bandwidth(uint32_t bandwidth);
        uint32_t bandwidth(void);

        uint16_t set_tx_attenuation(uint16_t attenuation);
        uint16_t tx_attenuation(void);

        void transmit( const int16_t *p_data, uint16_t num_samples );
    private:
        // sidekiq parameters
        uint64_t d_tx_freq;
        uint32_t d_tx_sample_rate;
        uint32_t d_tx_bandwidth;
        uint16_t d_tx_atten;

	STATUS d_srfs_src_status;

        // sockets
        int d_sock;
        int d_iq_sock;
        int d_iq_port;
        int d_src_port;
        state d_state; 
        hostent *d_server;
        sockaddr_in d_server_addr;
        sockaddr_in d_iq_server_addr;
        socklen_t d_addr_len;


	    // string to srfs_param_t map
	    typedef std::map<const std::string, srfs::srfs_param_t> param_map;
	    // pair string to srfs_param_t
	    typedef std::pair<const std::string, srfs::srfs_param_t> param_pair;

	    // map of valid sidekiq parameters and associated strings
	    param_map sidekiq_params;

	    // establishes connection to SRFS
	    void open_srfs();
	    // closes connection to SRFS
	    void close_srfs();
	    // initializes sidekiq_params map
	    void init_srfs_params(void);
	    // adds a parameter to the sidekiq_params map
	    void add_srfs_param( const std::string token,
				 srfs::SRFS_DATATYPES data_type,
				 void *p_value,
				 int64_t min_value,
				 int64_t max_value,
				 float resolution,
				 const std::string *p_strings );

	    // configures parameter specified by token to the value provided
	    void set_param( const std::string token, void *pValue );

	    // configures all sidekiq parameters
	    void config_src();	

        // sends a message to sidekiq
        void send_msg( char * );
        // receive a response from sidekiq
        int receive_msg( char *, int );



    };

    }
}

#endif
