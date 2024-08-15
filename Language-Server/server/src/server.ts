import { initialize } from "./methods/initialize";
import log from "./log";

interface Message {
	jsonrpc: string;
}

export interface RequestMessage extends Message {
	id: number | string;
	method: string;
	params?: unknown[] | object;
}

let buffer: string = '';
process.stdin.on('data', (chunk) => {
	buffer += chunk;

	while (true) {
		const lengthMatch = buffer.match(/Content-Length: (\d+)\r\n/);
		if (!lengthMatch) {
			return;
		} else {
			const contentLength: number = parseInt(lengthMatch[1], 10);
			const messageStart: number = buffer.indexOf('\r\n\r\n') + 4;

			if (buffer.length < messageStart + contentLength) {
				return;
			} else {
				const rawMessage: string = buffer.slice(messageStart, messageStart + contentLength);

				const message: RequestMessage = JSON.parse(rawMessage);
				log.write({ id: message.id, method:message.method });

				buffer = buffer.slice(messageStart + contentLength);
			}
		}
	}
});
