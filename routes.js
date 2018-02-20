const express = require('express');
const router = express.Router();
const base_path = './routes/';

// order is important
let routes = {
	express_static: {
		accesspoint: '/',
		route: express.static('public')
	},
	api: {
		accesspoint: '/api',
		route: require(base_path + 'api_route.js')
	}
};

for (let key in routes) {
	router.use(routes[key].accesspoint, routes[key].route);
}

module.exports = router;