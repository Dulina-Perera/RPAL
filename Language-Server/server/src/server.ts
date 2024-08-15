import { completion } from "./methods/textDocument/completion";
import { initialize } from "./methods/initialize";
import log from "./log";

type RequestMethod = (message: RequestMessage) => unknown;

interface Message {
	jsonrpc: string;
}

export interface RequestMessage extends Message {
	id: number | string;
	method: string;
	params?: unknown[] | object;
}

const methodLookup: Record<string, RequestMethod> = {
	initialize,
	"textDocument/completion": completion
};

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

				const method: RequestMethod = methodLookup[message.method];
				if (method) {
					respond(message.id, method(message));
				}

				buffer = buffer.slice(messageStart + contentLength);
			}
		}
	}
});

const respond = (id: RequestMessage['id'], result: unknown) => {
	const message: string = JSON.stringify({ id, result }, null, 2);
	const length: number = Buffer.byteLength(message, 'utf8');

	const header: string = `Content-Length: ${length}\r\n\r\n`;

	log.write(header + message);
	process.stdout.write(header + message);
}
