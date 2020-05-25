import React from 'react';
import './App.css';

class UserView extends React.Component {

    render() {
        const user = this.props.user;
        if (user) {
            return (
                <div className="userDetails">
                    <p>Name: {user.login}</p>
                    <p>Created on: {user.created_at.toString()}</p>
                    <p><img className="avatar" alt="avatar" src={user.avatar_url}/></p>
                </div>
            );
        } else {
            return <div></div>
        }
    }
}

class ReposView extends React.Component {

    render() {
        const repos = this.props.repos;
        if (repos) {
            const array = [];
            for (var i = 0; i < repos.size(); i++) {
                array.push(repos.get(i));
            }
            const listItems = array.map((repo) =>  <li key={repo.id}>{repo.full_name}</li>);
            return (
                <div className="repos">
                    <ul>{listItems}</ul>
                </div>
            );
        } else {
            return <div></div>
        }
    }
}


class NameForm extends React.Component {
    constructor(props) {
        super(props);
        this.state = {value: ''};
        this.handleChange = this.handleChange.bind(this);
        this.handleSubmit = this.handleSubmit.bind(this);
        this.callback = null;
    }

    handleChange(event) {
        this.setState({value: event.target.value});
    }

    handleSubmit(event) {
        if (this.state && this.state.value && this.state.value.length > 0) {
            if (!this.callback) {
                var that = this;
                // eslint-disable-next-line no-undef
                this.callback = new Module.JSGetReposCb({
                    on_error: function (error) {
                        console.error(error)
                    },
                    on_success: function (repos, user) {
                        that.props.onUserChange(user);
                        that.props.onReposChange(repos);
                    }
                });
            }

            window.xptuto.get_repos_for_user_name(this.state.value, this.callback);
        }
        event.preventDefault();
    }

    render() {
        return (
            <div className="userForm">
            <form onSubmit={this.handleSubmit}>
                <label>
                    Name:
                    <input type="text" value={this.state.value} onChange={this.handleChange} /> </label>
                <input type="submit" value="Submit"/>
            </form>
            </div>
        );
    }
}

class App extends React.Component {
    constructor(props) {
        super(props);
        this.state = {currentUser: null, currentRepos: null};
        this.handleUserChange = this.handleUserChange.bind(this);
        this.handleReposChange = this.handleReposChange.bind(this)
    }

    getInitialState() {
        return {currentUser: null};
    }

    handleUserChange(user) {
        this.setState({currentUser: user, currentRepos: this.state.currentRepos});
    }

    handleReposChange(repos) {
        this.setState({currentUser: this.state.currentUser, currentRepos: repos});
    }

    render() {
        return (
            <div className="App">
                <header className="App-header">
                    <NameForm onUserChange={this.handleUserChange} onReposChange={this.handleReposChange}/>

                    <UserView user={this.state.currentUser}/>
                    <ReposView repos={this.state.currentRepos} />
                </header>
            </div>
        );
    }
}

export default App;
