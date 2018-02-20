// imports
const http = require('http');
const express = require('express');
const logger = require('morgan');
const cfenv = require('cfenv');
const bodyParser = require('body-parser');
const helmet = require('helmet');
const router = require('./routes.js');

// globals
const appEnv = cfenv.getAppEnv();
const PORT = appEnv.isLocal ? process.env.npm_package_config_dev_port : (appEnv.port || process.env.PORT);
let app = express();

// logger
const logger_mode = (process.env.is_dev) ? 'dev' : 'combined';
app.use(logger(logger_mode));
appEnv.isLocal ? app.set('json spaces', 2) : app.set('json spaces', 0); // prettify json responses for dev

// security
app.use(helmet());
app.use(helmet.referrerPolicy({
	policy: 'no-referrer-when-downgrade'
}));
const required_domains = [
	"maxcdn.bootstrapcdn.com", "code.jquery.com", "cdnjs.cloudflare.com",
	"use.fontawesome.com", "cdn.jsdelivr.net", "ajax.googleapis.com",
	"fonts.googleapis.com", "fonts.gstatic.com", "raw.githubusercontent.com"
];
app.use(helmet.contentSecurityPolicy({
	directives: {
		// defaultSrc: ["'self'"],
		styleSrc: ["'self'", "'unsafe-inline'"].concat(required_domains),
		scriptSrc: ["'self'", "'unsafe-inline'", "'unsafe-eval'"].concat(required_domains),
		fontSrc: ["'self'"].concat(required_domains),
		imgSrc: ["'self'", 'blob:', 'data:'],
		childSrc: ["'self'"]
	}
}));

// data transmission and parsing
app.use(bodyParser.json({
	limit: '5Mb'
}));
app.use(bodyParser.urlencoded({
	extended: true
}));

// endpoints and routes
app.use(router);

// unhandled exception
process.on('uncaughtException', function(err) {
	console.error('uncaughtException: ' + err.message);
	console.error(err.stack);
	process.exit(1);
});

// server
http.createServer(app).listen(PORT, (d) => {
	console.info('Server started at: ', appEnv.isLocal ? 'port ' + PORT : appEnv.url);
});