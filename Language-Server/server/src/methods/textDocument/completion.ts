import { RequestMessage } from "../../server";

type CompletionItem = {
	label: string;
};

interface CompletionList {
	isIncomplete: boolean;
	items: CompletionItem[];
}

export const completion = (message: RequestMessage): CompletionList => {
	const completionList: CompletionList = {
		isIncomplete: false,
		items: [
			{ label: 'Typescript' },
			{ label: 'LSP' },
			{ label: 'Lua' }
		]
	};

	return completionList;
};
