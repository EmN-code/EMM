// The module 'vscode' contains the VS Code extensibility API
// Import the module and reference it with the alias vscode in your code below
import * as vscode from 'vscode';
import * as fs from 'fs';
import * as path from 'path';

export function activate(context: vscode.ExtensionContext) {
	context.subscriptions.push(
	  vscode.window.registerWebviewViewProvider(
		'iotpilot',
		new MyWebviewViewProvider(context)
	  )
	);
  }


class MyWebviewViewProvider implements vscode.WebviewViewProvider {
	constructor(private readonly context: vscode.ExtensionContext){}

	resolveWebviewView(webviewView: vscode.WebviewView, context: vscode.WebviewViewResolveContext, token: vscode.CancellationToken){
		webviewView.webview.options = {
			enableScripts: true,
			localResourceRoots: [this.context.extensionUri]
		};
		webviewView.webview.html = this.getWebviewContent(webviewView.webview);
	}

	private getWebviewContent(webview: vscode.Webview): string {
		const styleUri = webview.asWebviewUri(vscode.Uri.joinPath(this.context.extensionUri, 'media', 'style.css'));
		const scriptUri = webview.asWebviewUri(vscode.Uri.joinPath(this.context.extensionUri, 'media', 'main.js'));
    const iconUri = webview.asWebviewUri(vscode.Uri.joinPath(this.context.extensionUri, 'media', 'icon'));
    const HtmlPath = path.join(this.context.extensionUri.fsPath,'media','index.html');
    let html = fs.readFileSync(HtmlPath, 'utf8');
    html = html.replace('placeholder_style.css', styleUri.toString());
    html = html.replace('placeholder_main.js', scriptUri.toString());
    html = html.replace(/placeholder_icon/g, iconUri.toString());
		return html;
	}
}
// This method is called when your extension is deactivated
export function deactivate() {}
