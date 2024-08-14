import * as fs from 'fs';

const log: fs.WriteStream = fs.createWriteStream('/tmp/server.log', { flags: 'a' });

export default {
	write: (message: object | unknown) => {
		if (typeof message === 'object') {
			log.write(JSON.stringify(message, null, 2));
		} else {
			log.write(message);
		}
		log.write('\n');
	}
};
