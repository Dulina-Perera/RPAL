import * as fs from 'fs';
import { RequestMessage } from '../../server';

type CompletionItem = {
	label: string;
};

interface CompletionList {
	isIncomplete: boolean;
	items: CompletionItem[];
}

const words: string[] = fs.readFileSync('/usr/share/dict/words', 'utf8').split('\n');
const items: CompletionItem[] = words.map((word) => ({ label: word }));

export const completion = (message: RequestMessage): CompletionList => {
	const completionList: CompletionList = {
		isIncomplete: false,
		items
	};

	return completionList;
};
